/*************************************************************************/
/*  library_Fox_display.js                                             */
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

/*
 * Display Server listeners.
 * Keeps track of registered event listeners so it can remove them on shutdown.
 */
const FoxDisplayListeners = {
	$FoxDisplayListeners__deps: ['$FoxOS'],
	$FoxDisplayListeners__postset: 'FoxOS.atexit(function(resolve, reject) { FoxDisplayListeners.clear(); resolve(); });',
	$FoxDisplayListeners: {
		handlers: [],

		has: function (target, event, method, capture) {
			return FoxDisplayListeners.handlers.findIndex(function (e) {
				return e.target === target && e.event === event && e.method === method && e.capture === capture;
			}) !== -1;
		},

		add: function (target, event, method, capture) {
			if (FoxDisplayListeners.has(target, event, method, capture)) {
				return;
			}
			function Handler(p_target, p_event, p_method, p_capture) {
				this.target = p_target;
				this.event = p_event;
				this.method = p_method;
				this.capture = p_capture;
			}
			FoxDisplayListeners.handlers.push(new Handler(target, event, method, capture));
			target.addEventListener(event, method, capture);
		},

		clear: function () {
			FoxDisplayListeners.handlers.forEach(function (h) {
				h.target.removeEventListener(h.event, h.method, h.capture);
			});
			FoxDisplayListeners.handlers.length = 0;
		},
	},
};
mergeInto(LibraryManager.library, FoxDisplayListeners);

/*
 * Drag and drop handler.
 * This is pretty big, but basically detect dropped files on FoxConfig.canvas,
 * process them one by one (recursively for directories), and copies them to
 * the temporary FS path '/tmp/drop-[random]/' so it can be emitted as a Fox
 * event (that requires a string array of paths).
 *
 * NOTE: The temporary files are removed after the callback. This means that
 * deferred callbacks won't be able to access the files.
 */
const FoxDisplayDragDrop = {
	$FoxDisplayDragDrop__deps: ['$FS', '$FoxFS'],
	$FoxDisplayDragDrop: {
		promises: [],
		pending_files: [],

		add_entry: function (entry) {
			if (entry.isDirectory) {
				FoxDisplayDragDrop.add_dir(entry);
			} else if (entry.isFile) {
				FoxDisplayDragDrop.add_file(entry);
			} else {
				FoxRuntime.error('Unrecognized entry...', entry);
			}
		},

		add_dir: function (entry) {
			FoxDisplayDragDrop.promises.push(new Promise(function (resolve, reject) {
				const reader = entry.createReader();
				reader.readEntries(function (entries) {
					for (let i = 0; i < entries.length; i++) {
						FoxDisplayDragDrop.add_entry(entries[i]);
					}
					resolve();
				});
			}));
		},

		add_file: function (entry) {
			FoxDisplayDragDrop.promises.push(new Promise(function (resolve, reject) {
				entry.file(function (file) {
					const reader = new FileReader();
					reader.onload = function () {
						const f = {
							'path': file.relativePath || file.webkitRelativePath,
							'name': file.name,
							'type': file.type,
							'size': file.size,
							'data': reader.result,
						};
						if (!f['path']) {
							f['path'] = f['name'];
						}
						FoxDisplayDragDrop.pending_files.push(f);
						resolve();
					};
					reader.onerror = function () {
						FoxRuntime.print('Error reading file');
						reject();
					};
					reader.readAsArrayBuffer(file);
				}, function (err) {
					FoxRuntime.print('Error!');
					reject();
				});
			}));
		},

		process: function (resolve, reject) {
			if (FoxDisplayDragDrop.promises.length === 0) {
				resolve();
				return;
			}
			FoxDisplayDragDrop.promises.pop().then(function () {
				setTimeout(function () {
					FoxDisplayDragDrop.process(resolve, reject);
				}, 0);
			});
		},

		_process_event: function (ev, callback) {
			ev.preventDefault();
			if (ev.dataTransfer.items) {
				// Use DataTransferItemList interface to access the file(s)
				for (let i = 0; i < ev.dataTransfer.items.length; i++) {
					const item = ev.dataTransfer.items[i];
					let entry = null;
					if ('getAsEntry' in item) {
						entry = item.getAsEntry();
					} else if ('webkitGetAsEntry' in item) {
						entry = item.webkitGetAsEntry();
					}
					if (entry) {
						FoxDisplayDragDrop.add_entry(entry);
					}
				}
			} else {
				FoxRuntime.error('File upload not supported');
			}
			new Promise(FoxDisplayDragDrop.process).then(function () {
				const DROP = `/tmp/drop-${parseInt(Math.random() * (1 << 30), 10)}/`;
				const drops = [];
				const files = [];
				FS.mkdir(DROP);
				FoxDisplayDragDrop.pending_files.forEach((elem) => {
					const path = elem['path'];
					FoxFS.copy_to_fs(DROP + path, elem['data']);
					let idx = path.indexOf('/');
					if (idx === -1) {
						// Root file
						drops.push(DROP + path);
					} else {
						// Subdir
						const sub = path.substr(0, idx);
						idx = sub.indexOf('/');
						if (idx < 0 && drops.indexOf(DROP + sub) === -1) {
							drops.push(DROP + sub);
						}
					}
					files.push(DROP + path);
				});
				FoxDisplayDragDrop.promises = [];
				FoxDisplayDragDrop.pending_files = [];
				callback(drops);
				if (FoxConfig.persistent_drops) {
					// Delay removal at exit.
					FoxOS.atexit(function (resolve, reject) {
						FoxDisplayDragDrop.remove_drop(files, DROP);
						resolve();
					});
				} else {
					FoxDisplayDragDrop.remove_drop(files, DROP);
				}
			});
		},

		remove_drop: function (files, drop_path) {
			const dirs = [drop_path.substr(0, drop_path.length - 1)];
			// Remove temporary files
			files.forEach(function (file) {
				FS.unlink(file);
				let dir = file.replace(drop_path, '');
				let idx = dir.lastIndexOf('/');
				while (idx > 0) {
					dir = dir.substr(0, idx);
					if (dirs.indexOf(drop_path + dir) === -1) {
						dirs.push(drop_path + dir);
					}
					idx = dir.lastIndexOf('/');
				}
			});
			// Remove dirs.
			dirs.sort(function (a, b) {
				const al = (a.match(/\//g) || []).length;
				const bl = (b.match(/\//g) || []).length;
				if (al > bl) {
					return -1;
				} else if (al < bl) {
					return 1;
				}
				return 0;
			}).forEach(function (dir) {
				FS.rmdir(dir);
			});
		},

		handler: function (callback) {
			return function (ev) {
				FoxDisplayDragDrop._process_event(ev, callback);
			};
		},
	},
};
mergeInto(LibraryManager.library, FoxDisplayDragDrop);

const FoxDisplayVK = {

	$FoxDisplayVK__deps: ['$FoxRuntime', '$FoxConfig', '$FoxDisplayListeners'],
	$FoxDisplayVK__postset: 'FoxOS.atexit(function(resolve, reject) { FoxDisplayVK.clear(); resolve(); });',
	$FoxDisplayVK: {
		textinput: null,
		textarea: null,

		available: function () {
			return FoxConfig.virtual_keyboard && 'ontouchstart' in window;
		},

		init: function (input_cb) {
			function create(what) {
				const elem = document.createElement(what);
				elem.style.display = 'none';
				elem.style.position = 'absolute';
				elem.style.zIndex = '-1';
				elem.style.background = 'transparent';
				elem.style.padding = '0px';
				elem.style.margin = '0px';
				elem.style.overflow = 'hidden';
				elem.style.width = '0px';
				elem.style.height = '0px';
				elem.style.border = '0px';
				elem.style.outline = 'none';
				elem.readonly = true;
				elem.disabled = true;
				FoxDisplayListeners.add(elem, 'input', function (evt) {
					const c_str = FoxRuntime.allocString(elem.value);
					input_cb(c_str, elem.selectionEnd);
					FoxRuntime.free(c_str);
				}, false);
				FoxDisplayListeners.add(elem, 'blur', function (evt) {
					elem.style.display = 'none';
					elem.readonly = true;
					elem.disabled = true;
				}, false);
				FoxConfig.canvas.insertAdjacentElement('beforebegin', elem);
				return elem;
			}
			FoxDisplayVK.textinput = create('input');
			FoxDisplayVK.textarea = create('textarea');
			FoxDisplayVK.updateSize();
		},
		show: function (text, multiline, start, end) {
			if (!FoxDisplayVK.textinput || !FoxDisplayVK.textarea) {
				return;
			}
			if (FoxDisplayVK.textinput.style.display !== '' || FoxDisplayVK.textarea.style.display !== '') {
				FoxDisplayVK.hide();
			}
			FoxDisplayVK.updateSize();
			const elem = multiline ? FoxDisplayVK.textarea : FoxDisplayVK.textinput;
			elem.readonly = false;
			elem.disabled = false;
			elem.value = text;
			elem.style.display = 'block';
			elem.focus();
			elem.setSelectionRange(start, end);
		},
		hide: function () {
			if (!FoxDisplayVK.textinput || !FoxDisplayVK.textarea) {
				return;
			}
			[FoxDisplayVK.textinput, FoxDisplayVK.textarea].forEach(function (elem) {
				elem.blur();
				elem.style.display = 'none';
				elem.value = '';
			});
		},
		updateSize: function () {
			if (!FoxDisplayVK.textinput || !FoxDisplayVK.textarea) {
				return;
			}
			const rect = FoxConfig.canvas.getBoundingClientRect();
			function update(elem) {
				elem.style.left = `${rect.left}px`;
				elem.style.top = `${rect.top}px`;
				elem.style.width = `${rect.width}px`;
				elem.style.height = `${rect.height}px`;
			}
			update(FoxDisplayVK.textinput);
			update(FoxDisplayVK.textarea);
		},
		clear: function () {
			if (FoxDisplayVK.textinput) {
				FoxDisplayVK.textinput.remove();
				FoxDisplayVK.textinput = null;
			}
			if (FoxDisplayVK.textarea) {
				FoxDisplayVK.textarea.remove();
				FoxDisplayVK.textarea = null;
			}
		},
	},
};
mergeInto(LibraryManager.library, FoxDisplayVK);

/*
 * Display server cursor helper.
 * Keeps track of cursor status and custom shapes.
 */
const FoxDisplayCursor = {
	$FoxDisplayCursor__deps: ['$FoxOS', '$FoxConfig'],
	$FoxDisplayCursor__postset: 'FoxOS.atexit(function(resolve, reject) { FoxDisplayCursor.clear(); resolve(); });',
	$FoxDisplayCursor: {
		shape: 'auto',
		visible: true,
		cursors: {},
		set_style: function (style) {
			FoxConfig.canvas.style.cursor = style;
		},
		set_shape: function (shape) {
			FoxDisplayCursor.shape = shape;
			let css = shape;
			if (shape in FoxDisplayCursor.cursors) {
				const c = FoxDisplayCursor.cursors[shape];
				css = `url("${c.url}") ${c.x} ${c.y}, auto`;
			}
			if (FoxDisplayCursor.visible) {
				FoxDisplayCursor.set_style(css);
			}
		},
		clear: function () {
			FoxDisplayCursor.set_style('');
			FoxDisplayCursor.shape = 'auto';
			FoxDisplayCursor.visible = true;
			Object.keys(FoxDisplayCursor.cursors).forEach(function (key) {
				URL.revokeObjectURL(FoxDisplayCursor.cursors[key]);
				delete FoxDisplayCursor.cursors[key];
			});
		},
	},
};
mergeInto(LibraryManager.library, FoxDisplayCursor);

/*
 * Display Gamepad API helper.
 */
const FoxDisplayGamepads = {
	$FoxDisplayGamepads__deps: ['$FoxRuntime', '$FoxDisplayListeners'],
	$FoxDisplayGamepads: {
		samples: [],

		get_pads: function () {
			try {
				// Will throw in iframe when permission is denied.
				// Will throw/warn in the future for insecure contexts.
				// See https://github.com/w3c/gamepad/pull/120
				const pads = navigator.getGamepads();
				if (pads) {
					return pads;
				}
				return [];
			} catch (e) {
				return [];
			}
		},

		get_samples: function () {
			return FoxDisplayGamepads.samples;
		},

		get_sample: function (index) {
			const samples = FoxDisplayGamepads.samples;
			return index < samples.length ? samples[index] : null;
		},

		sample: function () {
			const pads = FoxDisplayGamepads.get_pads();
			const samples = [];
			for (let i = 0; i < pads.length; i++) {
				const pad = pads[i];
				if (!pad) {
					samples.push(null);
					continue;
				}
				const s = {
					standard: pad.mapping === 'standard',
					buttons: [],
					axes: [],
					connected: pad.connected,
				};
				for (let b = 0; b < pad.buttons.length; b++) {
					s.buttons.push(pad.buttons[b].value);
				}
				for (let a = 0; a < pad.axes.length; a++) {
					s.axes.push(pad.axes[a]);
				}
				samples.push(s);
			}
			FoxDisplayGamepads.samples = samples;
		},

		init: function (onchange) {
			FoxDisplayListeners.samples = [];
			function add(pad) {
				const guid = FoxDisplayGamepads.get_guid(pad);
				const c_id = FoxRuntime.allocString(pad.id);
				const c_guid = FoxRuntime.allocString(guid);
				onchange(pad.index, 1, c_id, c_guid);
				FoxRuntime.free(c_id);
				FoxRuntime.free(c_guid);
			}
			const pads = FoxDisplayGamepads.get_pads();
			for (let i = 0; i < pads.length; i++) {
				// Might be reserved space.
				if (pads[i]) {
					add(pads[i]);
				}
			}
			FoxDisplayListeners.add(window, 'gamepadconnected', function (evt) {
				add(evt.gamepad);
			}, false);
			FoxDisplayListeners.add(window, 'gamepaddisconnected', function (evt) {
				onchange(evt.gamepad.index, 0);
			}, false);
		},

		get_guid: function (pad) {
			if (pad.mapping) {
				return pad.mapping;
			}
			const ua = navigator.userAgent;
			let os = 'Unknown';
			if (ua.indexOf('Android') >= 0) {
				os = 'Android';
			} else if (ua.indexOf('Linux') >= 0) {
				os = 'Linux';
			} else if (ua.indexOf('iPhone') >= 0) {
				os = 'iOS';
			} else if (ua.indexOf('Macintosh') >= 0) {
				// Updated iPads will fall into this category.
				os = 'MacOSX';
			} else if (ua.indexOf('Windows') >= 0) {
				os = 'Windows';
			}

			const id = pad.id;
			// Chrom* style: NAME (Vendor: xxxx Product: xxxx)
			const exp1 = /vendor: ([0-9a-f]{4}) product: ([0-9a-f]{4})/i;
			// Firefox/Safari style (safari may remove leading zeores)
			const exp2 = /^([0-9a-f]+)-([0-9a-f]+)-/i;
			let vendor = '';
			let product = '';
			if (exp1.test(id)) {
				const match = exp1.exec(id);
				vendor = match[1].padStart(4, '0');
				product = match[2].padStart(4, '0');
			} else if (exp2.test(id)) {
				const match = exp2.exec(id);
				vendor = match[1].padStart(4, '0');
				product = match[2].padStart(4, '0');
			}
			if (!vendor || !product) {
				return `${os}Unknown`;
			}
			return os + vendor + product;
		},
	},
};
mergeInto(LibraryManager.library, FoxDisplayGamepads);

const FoxDisplayScreen = {
	$FoxDisplayScreen__deps: ['$FoxConfig', '$FoxOS', '$GL', 'emscripten_webgl_get_current_context'],
	$FoxDisplayScreen: {
		desired_size: [0, 0],
		hidpi: true,
		getPixelRatio: function () {
			return FoxDisplayScreen.hidpi ? window.devicePixelRatio || 1 : 1;
		},
		isFullscreen: function () {
			const elem = document.fullscreenElement || document.mozFullscreenElement
				|| document.webkitFullscreenElement || document.msFullscreenElement;
			if (elem) {
				return elem === FoxConfig.canvas;
			}
			// But maybe knowing the element is not supported.
			return document.fullscreen || document.mozFullScreen
				|| document.webkitIsFullscreen;
		},
		hasFullscreen: function () {
			return document.fullscreenEnabled || document.mozFullScreenEnabled
				|| document.webkitFullscreenEnabled;
		},
		requestFullscreen: function () {
			if (!FoxDisplayScreen.hasFullscreen()) {
				return 1;
			}
			const canvas = FoxConfig.canvas;
			try {
				const promise = (canvas.requestFullscreen || canvas.msRequestFullscreen
					|| canvas.mozRequestFullScreen || canvas.mozRequestFullscreen
					|| canvas.webkitRequestFullscreen
				).call(canvas);
				// Some browsers (Safari) return undefined.
				// For the standard ones, we need to catch it.
				if (promise) {
					promise.catch(function () {
						// nothing to do.
					});
				}
			} catch (e) {
				return 1;
			}
			return 0;
		},
		exitFullscreen: function () {
			if (!FoxDisplayScreen.isFullscreen()) {
				return 0;
			}
			try {
				const promise = document.exitFullscreen();
				if (promise) {
					promise.catch(function () {
						// nothing to do.
					});
				}
			} catch (e) {
				return 1;
			}
			return 0;
		},
		_updateGL: function () {
			const gl_context_handle = _emscripten_webgl_get_current_context(); // eslint-disable-line no-undef
			const gl = GL.getContext(gl_context_handle);
			if (gl) {
				GL.resizeOffscreenFramebuffer(gl);
			}
		},
		updateSize: function () {
			const isFullscreen = FoxDisplayScreen.isFullscreen();
			const wantsFullWindow = FoxConfig.canvas_resize_policy === 2;
			const noResize = FoxConfig.canvas_resize_policy === 0;
			const wwidth = FoxDisplayScreen.desired_size[0];
			const wheight = FoxDisplayScreen.desired_size[1];
			const canvas = FoxConfig.canvas;
			let width = wwidth;
			let height = wheight;
			if (noResize) {
				// Don't resize canvas, just update GL if needed.
				if (canvas.width !== width || canvas.height !== height) {
					FoxDisplayScreen.desired_size = [canvas.width, canvas.height];
					FoxDisplayScreen._updateGL();
					return 1;
				}
				return 0;
			}
			const scale = FoxDisplayScreen.getPixelRatio();
			if (isFullscreen || wantsFullWindow) {
				// We need to match screen size.
				width = window.innerWidth * scale;
				height = window.innerHeight * scale;
			}
			const csw = `${width / scale}px`;
			const csh = `${height / scale}px`;
			if (canvas.style.width !== csw || canvas.style.height !== csh || canvas.width !== width || canvas.height !== height) {
				// Size doesn't match.
				// Resize canvas, set correct CSS pixel size, update GL.
				canvas.width = width;
				canvas.height = height;
				canvas.style.width = csw;
				canvas.style.height = csh;
				FoxDisplayScreen._updateGL();
				return 1;
			}
			return 0;
		},
	},
};
mergeInto(LibraryManager.library, FoxDisplayScreen);

/**
 * Display server interface.
 *
 * Exposes all the functions needed by DisplayServer implementation.
 */
const FoxDisplay = {
	$FoxDisplay__deps: ['$FoxConfig', '$FoxRuntime', '$FoxDisplayCursor', '$FoxDisplayListeners', '$FoxDisplayDragDrop', '$FoxDisplayGamepads', '$FoxDisplayScreen', '$FoxDisplayVK'],
	$FoxDisplay: {
		window_icon: '',
		findDPI: function () {
			function testDPI(dpi) {
				return window.matchMedia(`(max-resolution: ${dpi}dpi)`).matches;
			}
			function bisect(low, high, func) {
				const mid = parseInt(((high - low) / 2) + low, 10);
				if (high - low <= 1) {
					return func(high) ? high : low;
				}
				if (func(mid)) {
					return bisect(low, mid, func);
				}
				return bisect(mid, high, func);
			}
			try {
				const dpi = bisect(0, 800, testDPI);
				return dpi >= 96 ? dpi : 96;
			} catch (e) {
				return 96;
			}
		},
	},

	Fox_js_display_is_swap_ok_cancel__sig: 'i',
	Fox_js_display_is_swap_ok_cancel: function () {
		const win = (['Windows', 'Win64', 'Win32', 'WinCE']);
		const plat = navigator.platform || '';
		if (win.indexOf(plat) !== -1) {
			return 1;
		}
		return 0;
	},

	Fox_js_display_alert__sig: 'vi',
	Fox_js_display_alert: function (p_text) {
		window.alert(FoxRuntime.parseString(p_text)); // eslint-disable-line no-alert
	},

	Fox_js_display_screen_dpi_get__sig: 'i',
	Fox_js_display_screen_dpi_get: function () {
		return FoxDisplay.findDPI();
	},

	Fox_js_display_pixel_ratio_get__sig: 'f',
	Fox_js_display_pixel_ratio_get: function () {
		return FoxDisplayScreen.getPixelRatio();
	},

	Fox_js_display_fullscreen_request__sig: 'i',
	Fox_js_display_fullscreen_request: function () {
		return FoxDisplayScreen.requestFullscreen();
	},

	Fox_js_display_fullscreen_exit__sig: 'i',
	Fox_js_display_fullscreen_exit: function () {
		return FoxDisplayScreen.exitFullscreen();
	},

	Fox_js_display_desired_size_set__sig: 'vii',
	Fox_js_display_desired_size_set: function (width, height) {
		FoxDisplayScreen.desired_size = [width, height];
		FoxDisplayScreen.updateSize();
	},

	Fox_js_display_size_update__sig: 'i',
	Fox_js_display_size_update: function () {
		const updated = FoxDisplayScreen.updateSize();
		if (updated) {
			FoxDisplayVK.updateSize();
		}
		return updated;
	},

	Fox_js_display_screen_size_get__sig: 'vii',
	Fox_js_display_screen_size_get: function (width, height) {
		const scale = FoxDisplayScreen.getPixelRatio();
		FoxRuntime.setHeapValue(width, window.screen.width * scale, 'i32');
		FoxRuntime.setHeapValue(height, window.screen.height * scale, 'i32');
	},

	Fox_js_display_window_size_get: function (p_width, p_height) {
		FoxRuntime.setHeapValue(p_width, FoxConfig.canvas.width, 'i32');
		FoxRuntime.setHeapValue(p_height, FoxConfig.canvas.height, 'i32');
	},

	Fox_js_display_compute_position: function (x, y, r_x, r_y) {
		const canvas = FoxConfig.canvas;
		const rect = canvas.getBoundingClientRect();
		const rw = canvas.width / rect.width;
		const rh = canvas.height / rect.height;
		FoxRuntime.setHeapValue(r_x, (x - rect.x) * rw, 'i32');
		FoxRuntime.setHeapValue(r_y, (y - rect.y) * rh, 'i32');
	},

	Fox_js_display_has_webgl__sig: 'ii',
	Fox_js_display_has_webgl: function (p_version) {
		if (p_version !== 1 && p_version !== 2) {
			return false;
		}
		try {
			return !!document.createElement('canvas').getContext(p_version === 2 ? 'webgl2' : 'webgl');
		} catch (e) { /* Not available */ }
		return false;
	},

	/*
	 * Canvas
	 */
	Fox_js_display_canvas_focus__sig: 'v',
	Fox_js_display_canvas_focus: function () {
		FoxConfig.canvas.focus();
	},

	Fox_js_display_canvas_is_focused__sig: 'i',
	Fox_js_display_canvas_is_focused: function () {
		return document.activeElement === FoxConfig.canvas;
	},

	/*
	 * Touchscreen
	 */
	Fox_js_display_touchscreen_is_available__sig: 'i',
	Fox_js_display_touchscreen_is_available: function () {
		return 'ontouchstart' in window;
	},

	/*
	 * Clipboard
	 */
	Fox_js_display_clipboard_set__sig: 'ii',
	Fox_js_display_clipboard_set: function (p_text) {
		const text = FoxRuntime.parseString(p_text);
		if (!navigator.clipboard || !navigator.clipboard.writeText) {
			return 1;
		}
		navigator.clipboard.writeText(text).catch(function (e) {
			// Setting OS clipboard is only possible from an input callback.
			FoxRuntime.error('Setting OS clipboard is only possible from an input callback for the HTML5 plafrom. Exception:', e);
		});
		return 0;
	},

	Fox_js_display_clipboard_get__sig: 'ii',
	Fox_js_display_clipboard_get: function (callback) {
		const func = FoxRuntime.get_func(callback);
		try {
			navigator.clipboard.readText().then(function (result) {
				const ptr = FoxRuntime.allocString(result);
				func(ptr);
				FoxRuntime.free(ptr);
			}).catch(function (e) {
				// Fail graciously.
			});
		} catch (e) {
			// Fail graciously.
		}
	},

	/*
	 * Window
	 */
	Fox_js_display_window_title_set__sig: 'vi',
	Fox_js_display_window_title_set: function (p_data) {
		document.title = FoxRuntime.parseString(p_data);
	},

	Fox_js_display_window_icon_set__sig: 'vii',
	Fox_js_display_window_icon_set: function (p_ptr, p_len) {
		let link = document.getElementById('-gd-engine-icon');
		if (link === null) {
			link = document.createElement('link');
			link.rel = 'icon';
			link.id = '-gd-engine-icon';
			document.head.appendChild(link);
		}
		const old_icon = FoxDisplay.window_icon;
		const png = new Blob([FoxRuntime.heapSlice(HEAPU8, p_ptr, p_len)], { type: 'image/png' });
		FoxDisplay.window_icon = URL.createObjectURL(png);
		link.href = FoxDisplay.window_icon;
		if (old_icon) {
			URL.revokeObjectURL(old_icon);
		}
	},

	/*
	 * Cursor
	 */
	Fox_js_display_cursor_set_visible__sig: 'vi',
	Fox_js_display_cursor_set_visible: function (p_visible) {
		const visible = p_visible !== 0;
		if (visible === FoxDisplayCursor.visible) {
			return;
		}
		FoxDisplayCursor.visible = visible;
		if (visible) {
			FoxDisplayCursor.set_shape(FoxDisplayCursor.shape);
		} else {
			FoxDisplayCursor.set_style('none');
		}
	},

	Fox_js_display_cursor_is_hidden__sig: 'i',
	Fox_js_display_cursor_is_hidden: function () {
		return !FoxDisplayCursor.visible;
	},

	Fox_js_display_cursor_set_shape__sig: 'vi',
	Fox_js_display_cursor_set_shape: function (p_string) {
		FoxDisplayCursor.set_shape(FoxRuntime.parseString(p_string));
	},

	Fox_js_display_cursor_set_custom_shape__sig: 'viiiii',
	Fox_js_display_cursor_set_custom_shape: function (p_shape, p_ptr, p_len, p_hotspot_x, p_hotspot_y) {
		const shape = FoxRuntime.parseString(p_shape);
		const old_shape = FoxDisplayCursor.cursors[shape];
		if (p_len > 0) {
			const png = new Blob([FoxRuntime.heapSlice(HEAPU8, p_ptr, p_len)], { type: 'image/png' });
			const url = URL.createObjectURL(png);
			FoxDisplayCursor.cursors[shape] = {
				url: url,
				x: p_hotspot_x,
				y: p_hotspot_y,
			};
		} else {
			delete FoxDisplayCursor.cursors[shape];
		}
		if (shape === FoxDisplayCursor.shape) {
			FoxDisplayCursor.set_shape(FoxDisplayCursor.shape);
		}
		if (old_shape) {
			URL.revokeObjectURL(old_shape.url);
		}
	},

	/*
	 * Listeners
	 */
	Fox_js_display_notification_cb__sig: 'viiiii',
	Fox_js_display_notification_cb: function (callback, p_enter, p_exit, p_in, p_out) {
		const canvas = FoxConfig.canvas;
		const func = FoxRuntime.get_func(callback);
		const notif = [p_enter, p_exit, p_in, p_out];
		['mouseover', 'mouseleave', 'focus', 'blur'].forEach(function (evt_name, idx) {
			FoxDisplayListeners.add(canvas, evt_name, function () {
				func(notif[idx]);
			}, true);
		});
	},

	Fox_js_display_paste_cb__sig: 'vi',
	Fox_js_display_paste_cb: function (callback) {
		const func = FoxRuntime.get_func(callback);
		FoxDisplayListeners.add(window, 'paste', function (evt) {
			const text = evt.clipboardData.getData('text');
			const ptr = FoxRuntime.allocString(text);
			func(ptr);
			FoxRuntime.free(ptr);
		}, false);
	},

	Fox_js_display_drop_files_cb__sig: 'vi',
	Fox_js_display_drop_files_cb: function (callback) {
		const func = FoxRuntime.get_func(callback);
		const dropFiles = function (files) {
			const args = files || [];
			if (!args.length) {
				return;
			}
			const argc = args.length;
			const argv = FoxRuntime.allocStringArray(args);
			func(argv, argc);
			FoxRuntime.freeStringArray(argv, argc);
		};
		const canvas = FoxConfig.canvas;
		FoxDisplayListeners.add(canvas, 'dragover', function (ev) {
			// Prevent default behavior (which would try to open the file(s))
			ev.preventDefault();
		}, false);
		FoxDisplayListeners.add(canvas, 'drop', FoxDisplayDragDrop.handler(dropFiles));
	},

	Fox_js_display_setup_canvas__sig: 'viiii',
	Fox_js_display_setup_canvas: function (p_width, p_height, p_fullscreen, p_hidpi) {
		const canvas = FoxConfig.canvas;
		FoxDisplayListeners.add(canvas, 'contextmenu', function (ev) {
			ev.preventDefault();
		}, false);
		FoxDisplayListeners.add(canvas, 'webglcontextlost', function (ev) {
			alert('WebGL context lost, please reload the page'); // eslint-disable-line no-alert
			ev.preventDefault();
		}, false);
		FoxDisplayScreen.hidpi = !!p_hidpi;
		switch (FoxConfig.canvas_resize_policy) {
		case 0: // None
			FoxDisplayScreen.desired_size = [canvas.width, canvas.height];
			break;
		case 1: // Project
			FoxDisplayScreen.desired_size = [p_width, p_height];
			break;
		default: // Full window
			// Ensure we display in the right place, the size will be handled by updateSize
			canvas.style.position = 'absolute';
			canvas.style.top = 0;
			canvas.style.left = 0;
			break;
		}
		FoxDisplayScreen.updateSize();
		if (p_fullscreen) {
			FoxDisplayScreen.requestFullscreen();
		}
	},

	/*
	 * Virtual Keyboard
	 */
	Fox_js_display_vk_show__sig: 'viiii',
	Fox_js_display_vk_show: function (p_text, p_multiline, p_start, p_end) {
		const text = FoxRuntime.parseString(p_text);
		const start = p_start > 0 ? p_start : 0;
		const end = p_end > 0 ? p_end : start;
		FoxDisplayVK.show(text, p_multiline, start, end);
	},

	Fox_js_display_vk_hide__sig: 'v',
	Fox_js_display_vk_hide: function () {
		FoxDisplayVK.hide();
	},

	Fox_js_display_vk_available__sig: 'i',
	Fox_js_display_vk_available: function () {
		return FoxDisplayVK.available();
	},

	Fox_js_display_vk_cb__sig: 'vi',
	Fox_js_display_vk_cb: function (p_input_cb) {
		const input_cb = FoxRuntime.get_func(p_input_cb);
		if (FoxDisplayVK.available()) {
			FoxDisplayVK.init(input_cb);
		}
	},

	/*
	 * Gamepads
	 */
	Fox_js_display_gamepad_cb__sig: 'vi',
	Fox_js_display_gamepad_cb: function (change_cb) {
		const onchange = FoxRuntime.get_func(change_cb);
		FoxDisplayGamepads.init(onchange);
	},

	Fox_js_display_gamepad_sample_count__sig: 'i',
	Fox_js_display_gamepad_sample_count: function () {
		return FoxDisplayGamepads.get_samples().length;
	},

	Fox_js_display_gamepad_sample__sig: 'i',
	Fox_js_display_gamepad_sample: function () {
		FoxDisplayGamepads.sample();
		return 0;
	},

	Fox_js_display_gamepad_sample_get__sig: 'iiiiiii',
	Fox_js_display_gamepad_sample_get: function (p_index, r_btns, r_btns_num, r_axes, r_axes_num, r_standard) {
		const sample = FoxDisplayGamepads.get_sample(p_index);
		if (!sample || !sample.connected) {
			return 1;
		}
		const btns = sample.buttons;
		const btns_len = btns.length < 16 ? btns.length : 16;
		for (let i = 0; i < btns_len; i++) {
			FoxRuntime.setHeapValue(r_btns + (i << 2), btns[i], 'float');
		}
		FoxRuntime.setHeapValue(r_btns_num, btns_len, 'i32');
		const axes = sample.axes;
		const axes_len = axes.length < 10 ? axes.length : 10;
		for (let i = 0; i < axes_len; i++) {
			FoxRuntime.setHeapValue(r_axes + (i << 2), axes[i], 'float');
		}
		FoxRuntime.setHeapValue(r_axes_num, axes_len, 'i32');
		const is_standard = sample.standard ? 1 : 0;
		FoxRuntime.setHeapValue(r_standard, is_standard, 'i32');
		return 0;
	},
};

autoAddDeps(FoxDisplay, '$FoxDisplay');
mergeInto(LibraryManager.library, FoxDisplay);
