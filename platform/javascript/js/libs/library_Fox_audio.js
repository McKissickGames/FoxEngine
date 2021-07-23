/*************************************************************************/
/*  library_Fox_audio.js                                               */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           Fox ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2021 Fox Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

const FoxAudio = {
	$FoxAudio__deps: ['$FoxRuntime', '$FoxOS'],
	$FoxAudio: {
		ctx: null,
		input: null,
		driver: null,
		interval: 0,

		init: function (mix_rate, latency, onstatechange, onlatencyupdate) {
			const ctx = new (window.AudioContext || window.webkitAudioContext)({
				sampleRate: mix_rate,
				// latencyHint: latency / 1000 // Do not specify, leave 'interactive' for good performance.
			});
			FoxAudio.ctx = ctx;
			ctx.onstatechange = function () {
				let state = 0;
				switch (ctx.state) {
				case 'suspended':
					state = 0;
					break;
				case 'running':
					state = 1;
					break;
				case 'closed':
					state = 2;
					break;

					// no default
				}
				onstatechange(state);
			};
			ctx.onstatechange(); // Immediately notify state.
			// Update computed latency
			FoxAudio.interval = setInterval(function () {
				let computed_latency = 0;
				if (ctx.baseLatency) {
					computed_latency += FoxAudio.ctx.baseLatency;
				}
				if (ctx.outputLatency) {
					computed_latency += FoxAudio.ctx.outputLatency;
				}
				onlatencyupdate(computed_latency);
			}, 1000);
			FoxOS.atexit(FoxAudio.close_async);
			return ctx.destination.channelCount;
		},

		create_input: function (callback) {
			if (FoxAudio.input) {
				return 0; // Already started.
			}
			function gotMediaInput(stream) {
				try {
					FoxAudio.input = FoxAudio.ctx.createMediaStreamSource(stream);
					callback(FoxAudio.input);
				} catch (e) {
					FoxRuntime.error('Failed creaating input.', e);
				}
			}
			if (navigator.mediaDevices && navigator.mediaDevices.getUserMedia) {
				navigator.mediaDevices.getUserMedia({
					'audio': true,
				}).then(gotMediaInput, function (e) {
					FoxRuntime.error('Error getting user media.', e);
				});
			} else {
				if (!navigator.getUserMedia) {
					navigator.getUserMedia = navigator.webkitGetUserMedia || navigator.mozGetUserMedia;
				}
				if (!navigator.getUserMedia) {
					FoxRuntime.error('getUserMedia not available.');
					return 1;
				}
				navigator.getUserMedia({
					'audio': true,
				}, gotMediaInput, function (e) {
					FoxRuntime.print(e);
				});
			}
			return 0;
		},

		close_async: function (resolve, reject) {
			const ctx = FoxAudio.ctx;
			FoxAudio.ctx = null;
			// Audio was not initialized.
			if (!ctx) {
				resolve();
				return;
			}
			// Remove latency callback
			if (FoxAudio.interval) {
				clearInterval(FoxAudio.interval);
				FoxAudio.interval = 0;
			}
			// Disconnect input, if it was started.
			if (FoxAudio.input) {
				FoxAudio.input.disconnect();
				FoxAudio.input = null;
			}
			// Disconnect output
			let closed = Promise.resolve();
			if (FoxAudio.driver) {
				closed = FoxAudio.driver.close();
			}
			closed.then(function () {
				return ctx.close();
			}).then(function () {
				ctx.onstatechange = null;
				resolve();
			}).catch(function (e) {
				ctx.onstatechange = null;
				FoxRuntime.error('Error closing AudioContext', e);
				resolve();
			});
		},
	},

	Fox_audio_is_available__sig: 'i',
	Fox_audio_is_available__proxy: 'sync',
	Fox_audio_is_available: function () {
		if (!(window.AudioContext || window.webkitAudioContext)) {
			return 0;
		}
		return 1;
	},

	Fox_audio_init__sig: 'iiiii',
	Fox_audio_init: function (p_mix_rate, p_latency, p_state_change, p_latency_update) {
		const statechange = FoxRuntime.get_func(p_state_change);
		const latencyupdate = FoxRuntime.get_func(p_latency_update);
		return FoxAudio.init(p_mix_rate, p_latency, statechange, latencyupdate);
	},

	Fox_audio_resume__sig: 'v',
	Fox_audio_resume: function () {
		if (FoxAudio.ctx && FoxAudio.ctx.state !== 'running') {
			FoxAudio.ctx.resume();
		}
	},

	Fox_audio_capture_start__proxy: 'sync',
	Fox_audio_capture_start__sig: 'i',
	Fox_audio_capture_start: function () {
		return FoxAudio.create_input(function (input) {
			input.connect(FoxAudio.driver.get_node());
		});
	},

	Fox_audio_capture_stop__proxy: 'sync',
	Fox_audio_capture_stop__sig: 'v',
	Fox_audio_capture_stop: function () {
		if (FoxAudio.input) {
			const tracks = FoxAudio.input['mediaStream']['getTracks']();
			for (let i = 0; i < tracks.length; i++) {
				tracks[i]['stop']();
			}
			FoxAudio.input.disconnect();
			FoxAudio.input = null;
		}
	},
};

autoAddDeps(FoxAudio, '$FoxAudio');
mergeInto(LibraryManager.library, FoxAudio);

/**
 * The AudioWorklet API driver, used when threads are available.
 */
const FoxAudioWorklet = {
	$FoxAudioWorklet__deps: ['$FoxAudio', '$FoxConfig'],
	$FoxAudioWorklet: {
		promise: null,
		worklet: null,

		create: function (channels) {
			const path = FoxConfig.locate_file('Fox.audio.worklet.js');
			FoxAudioWorklet.promise = FoxAudio.ctx.audioWorklet.addModule(path).then(function () {
				FoxAudioWorklet.worklet = new AudioWorkletNode(
					FoxAudio.ctx,
					'Fox-processor',
					{
						'outputChannelCount': [channels],
					},
				);
				return Promise.resolve();
			});
			FoxAudio.driver = FoxAudioWorklet;
		},

		start: function (in_buf, out_buf, state) {
			FoxAudioWorklet.promise.then(function () {
				const node = FoxAudioWorklet.worklet;
				node.connect(FoxAudio.ctx.destination);
				node.port.postMessage({
					'cmd': 'start',
					'data': [state, in_buf, out_buf],
				});
				node.port.onmessage = function (event) {
					FoxRuntime.error(event.data);
				};
			});
		},

		get_node: function () {
			return FoxAudioWorklet.worklet;
		},

		close: function () {
			return new Promise(function (resolve, reject) {
				if (FoxAudioWorklet.promise === null) {
					return;
				}
				FoxAudioWorklet.promise.then(function () {
					FoxAudioWorklet.worklet.port.postMessage({
						'cmd': 'stop',
						'data': null,
					});
					FoxAudioWorklet.worklet.disconnect();
					FoxAudioWorklet.worklet = null;
					FoxAudioWorklet.promise = null;
					resolve();
				}).catch(function (err) { /* aborted? */ });
			});
		},
	},

	Fox_audio_worklet_create__sig: 'vi',
	Fox_audio_worklet_create: function (channels) {
		FoxAudioWorklet.create(channels);
	},

	Fox_audio_worklet_start__sig: 'viiiii',
	Fox_audio_worklet_start: function (p_in_buf, p_in_size, p_out_buf, p_out_size, p_state) {
		const out_buffer = FoxRuntime.heapSub(HEAPF32, p_out_buf, p_out_size);
		const in_buffer = FoxRuntime.heapSub(HEAPF32, p_in_buf, p_in_size);
		const state = FoxRuntime.heapSub(HEAP32, p_state, 4);
		FoxAudioWorklet.start(in_buffer, out_buffer, state);
	},

	Fox_audio_worklet_state_wait__sig: 'iiii',
	Fox_audio_worklet_state_wait: function (p_state, p_idx, p_expected, p_timeout) {
		Atomics.wait(HEAP32, (p_state >> 2) + p_idx, p_expected, p_timeout);
		return Atomics.load(HEAP32, (p_state >> 2) + p_idx);
	},

	Fox_audio_worklet_state_add__sig: 'iiii',
	Fox_audio_worklet_state_add: function (p_state, p_idx, p_value) {
		return Atomics.add(HEAP32, (p_state >> 2) + p_idx, p_value);
	},

	Fox_audio_worklet_state_get__sig: 'iii',
	Fox_audio_worklet_state_get: function (p_state, p_idx) {
		return Atomics.load(HEAP32, (p_state >> 2) + p_idx);
	},
};

autoAddDeps(FoxAudioWorklet, '$FoxAudioWorklet');
mergeInto(LibraryManager.library, FoxAudioWorklet);

/*
 * The deprecated ScriptProcessorNode API, used when threads are disabled.
 */
const FoxAudioScript = {
	$FoxAudioScript__deps: ['$FoxAudio'],
	$FoxAudioScript: {
		script: null,

		create: function (buffer_length, channel_count) {
			FoxAudioScript.script = FoxAudio.ctx.createScriptProcessor(buffer_length, 2, channel_count);
			FoxAudio.driver = FoxAudioScript;
			return FoxAudioScript.script.bufferSize;
		},

		start: function (p_in_buf, p_in_size, p_out_buf, p_out_size, onprocess) {
			FoxAudioScript.script.onaudioprocess = function (event) {
				// Read input
				const inb = FoxRuntime.heapSub(HEAPF32, p_in_buf, p_in_size);
				const input = event.inputBuffer;
				if (FoxAudio.input) {
					const inlen = input.getChannelData(0).length;
					for (let ch = 0; ch < 2; ch++) {
						const data = input.getChannelData(ch);
						for (let s = 0; s < inlen; s++) {
							inb[s * 2 + ch] = data[s];
						}
					}
				}

				// Let Fox process the input/output.
				onprocess();

				// Write the output.
				const outb = FoxRuntime.heapSub(HEAPF32, p_out_buf, p_out_size);
				const output = event.outputBuffer;
				const channels = output.numberOfChannels;
				for (let ch = 0; ch < channels; ch++) {
					const data = output.getChannelData(ch);
					// Loop through samples and assign computed values.
					for (let sample = 0; sample < data.length; sample++) {
						data[sample] = outb[sample * channels + ch];
					}
				}
			};
			FoxAudioScript.script.connect(FoxAudio.ctx.destination);
		},

		get_node: function () {
			return FoxAudioScript.script;
		},

		close: function () {
			return new Promise(function (resolve, reject) {
				FoxAudioScript.script.disconnect();
				FoxAudioScript.script.onaudioprocess = null;
				FoxAudioScript.script = null;
				resolve();
			});
		},
	},

	Fox_audio_script_create__sig: 'iii',
	Fox_audio_script_create: function (buffer_length, channel_count) {
		return FoxAudioScript.create(buffer_length, channel_count);
	},

	Fox_audio_script_start__sig: 'viiiii',
	Fox_audio_script_start: function (p_in_buf, p_in_size, p_out_buf, p_out_size, p_cb) {
		const onprocess = FoxRuntime.get_func(p_cb);
		FoxAudioScript.start(p_in_buf, p_in_size, p_out_buf, p_out_size, onprocess);
	},
};

autoAddDeps(FoxAudioScript, '$FoxAudioScript');
mergeInto(LibraryManager.library, FoxAudioScript);
