/*************************************************************************/
/*  library_Fox_webxr.js                                               */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           Fox ENGINE                                */
/*                      https://Foxengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2020 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2020 Fox Engine contributors (cf. AUTHORS.md).   */
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
const FoxWebXR = {
	$FoxWebXR__deps: ['$Browser', '$GL', '$FoxRuntime'],
	$FoxWebXR: {
		gl: null,

		texture_ids: [null, null],
		textures: [null, null],

		session: null,
		space: null,
		frame: null,
		pose: null,

		// Monkey-patch the requestAnimationFrame() used by Emscripten for the main
		// loop, so that we can swap it out for XRSession.requestAnimationFrame()
		// when an XR session is started.
		orig_requestAnimationFrame: null,
		requestAnimationFrame: (callback) => {
			if (FoxWebXR.session && FoxWebXR.space) {
				const onFrame = function (time, frame) {
					FoxWebXR.frame = frame;
					FoxWebXR.pose = frame.getViewerPose(FoxWebXR.space);
					callback(time);
					FoxWebXR.frame = null;
					FoxWebXR.pose = null;
				};
				FoxWebXR.session.requestAnimationFrame(onFrame);
			} else {
				FoxWebXR.orig_requestAnimationFrame(callback);
			}
		},
		monkeyPatchRequestAnimationFrame: (enable) => {
			if (FoxWebXR.orig_requestAnimationFrame === null) {
				FoxWebXR.orig_requestAnimationFrame = Browser.requestAnimationFrame;
			}
			Browser.requestAnimationFrame = enable
				? FoxWebXR.requestAnimationFrame : FoxWebXR.orig_requestAnimationFrame;
		},
		pauseResumeMainLoop: () => {
			// Once both FoxWebXR.session and FoxWebXR.space are set or
			// unset, our monkey-patched requestAnimationFrame() should be
			// enabled or disabled. When using the WebXR API Emulator, this
			// gets picked up automatically, however, in the Oculus Browser
			// on the Quest, we need to pause and resume the main loop.
			Browser.mainLoop.pause();
			window.setTimeout(function () {
				Browser.mainLoop.resume();
			}, 0);
		},

		// Some custom WebGL code for blitting our eye textures to the
		// framebuffer we get from WebXR.
		shaderProgram: null,
		programInfo: null,
		buffer: null,
		// Vertex shader source.
		vsSource: `
			const vec2 scale = vec2(0.5, 0.5);
			attribute vec4 aVertexPosition;

			varying highp vec2 vTextureCoord;

			void main () {
				gl_Position = aVertexPosition;
				vTextureCoord = aVertexPosition.xy * scale + scale;
			}
		`,
		// Fragment shader source.
		fsSource: `
			varying highp vec2 vTextureCoord;

			uniform sampler2D uSampler;

			void main() {
				gl_FragColor = texture2D(uSampler, vTextureCoord);
			}
		`,

		initShaderProgram: (gl, vsSource, fsSource) => {
			const vertexShader = FoxWebXR.loadShader(gl, gl.VERTEX_SHADER, vsSource);
			const fragmentShader = FoxWebXR.loadShader(gl, gl.FRAGMENT_SHADER, fsSource);

			const shaderProgram = gl.createProgram();
			gl.attachShader(shaderProgram, vertexShader);
			gl.attachShader(shaderProgram, fragmentShader);
			gl.linkProgram(shaderProgram);

			if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
				FoxRuntime.error(`Unable to initialize the shader program: ${gl.getProgramInfoLog(shaderProgram)}`);
				return null;
			}

			return shaderProgram;
		},
		loadShader: (gl, type, source) => {
			const shader = gl.createShader(type);
			gl.shaderSource(shader, source);
			gl.compileShader(shader);

			if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
				FoxRuntime.error(`An error occurred compiling the shader: ${gl.getShaderInfoLog(shader)}`);
				gl.deleteShader(shader);
				return null;
			}

			return shader;
		},
		initBuffer: (gl) => {
			const positionBuffer = gl.createBuffer();
			gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
			const positions = [
				-1.0, -1.0,
				1.0, -1.0,
				-1.0, 1.0,
				1.0, 1.0,
			];
			gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);
			return positionBuffer;
		},
		blitTexture: (gl, texture) => {
			if (FoxWebXR.shaderProgram === null) {
				FoxWebXR.shaderProgram = FoxWebXR.initShaderProgram(gl, FoxWebXR.vsSource, FoxWebXR.fsSource);
				FoxWebXR.programInfo = {
					program: FoxWebXR.shaderProgram,
					attribLocations: {
						vertexPosition: gl.getAttribLocation(FoxWebXR.shaderProgram, 'aVertexPosition'),
					},
					uniformLocations: {
						uSampler: gl.getUniformLocation(FoxWebXR.shaderProgram, 'uSampler'),
					},
				};
				FoxWebXR.buffer = FoxWebXR.initBuffer(gl);
			}

			const orig_program = gl.getParameter(gl.CURRENT_PROGRAM);
			gl.useProgram(FoxWebXR.shaderProgram);

			gl.bindBuffer(gl.ARRAY_BUFFER, FoxWebXR.buffer);
			gl.vertexAttribPointer(FoxWebXR.programInfo.attribLocations.vertexPosition, 2, gl.FLOAT, false, 0, 0);
			gl.enableVertexAttribArray(FoxWebXR.programInfo.attribLocations.vertexPosition);

			gl.activeTexture(gl.TEXTURE0);
			gl.bindTexture(gl.TEXTURE_2D, texture);
			gl.uniform1i(FoxWebXR.programInfo.uniformLocations.uSampler, 0);

			gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);

			// Restore state.
			gl.bindTexture(gl.TEXTURE_2D, null);
			gl.disableVertexAttribArray(FoxWebXR.programInfo.attribLocations.vertexPosition);
			gl.bindBuffer(gl.ARRAY_BUFFER, null);
			gl.useProgram(orig_program);
		},

		// Holds the controllers list between function calls.
		controllers: [],

		// Updates controllers array, where the left hand (or sole tracker) is
		// the first element, and the right hand is the second element, and any
		// others placed at the 3rd position and up.
		sampleControllers: () => {
			if (!FoxWebXR.session || !FoxWebXR.frame) {
				return;
			}

			let other_index = 2;
			const controllers = [];
			FoxWebXR.session.inputSources.forEach((input_source) => {
				if (input_source.targetRayMode === 'tracked-pointer') {
					if (input_source.handedness === 'right') {
						controllers[1] = input_source;
					} else if (input_source.handedness === 'left' || !controllers[0]) {
						controllers[0] = input_source;
					}
				} else {
					controllers[other_index++] = input_source;
				}
			});
			FoxWebXR.controllers = controllers;
		},

		getControllerId: (input_source) => FoxWebXR.controllers.indexOf(input_source),
	},

	Fox_webxr_is_supported__proxy: 'sync',
	Fox_webxr_is_supported__sig: 'i',
	Fox_webxr_is_supported: function () {
		return !!navigator.xr;
	},

	Fox_webxr_is_session_supported__proxy: 'sync',
	Fox_webxr_is_session_supported__sig: 'vii',
	Fox_webxr_is_session_supported: function (p_session_mode, p_callback) {
		const session_mode = FoxRuntime.parseString(p_session_mode);
		const cb = FoxRuntime.get_func(p_callback);
		if (navigator.xr) {
			navigator.xr.isSessionSupported(session_mode).then(function (supported) {
				const c_str = FoxRuntime.allocString(session_mode);
				cb(c_str, supported ? 1 : 0);
				FoxRuntime.free(c_str);
			});
		} else {
			const c_str = FoxRuntime.allocString(session_mode);
			cb(c_str, 0);
			FoxRuntime.free(c_str);
		}
	},

	Fox_webxr_initialize__deps: ['emscripten_webgl_get_current_context'],
	Fox_webxr_initialize__proxy: 'sync',
	Fox_webxr_initialize__sig: 'viiiiiiiiii',
	Fox_webxr_initialize: function (p_session_mode, p_required_features, p_optional_features, p_requested_reference_spaces, p_on_session_started, p_on_session_ended, p_on_session_failed, p_on_controller_changed, p_on_input_event, p_on_simple_event) {
		FoxWebXR.monkeyPatchRequestAnimationFrame(true);

		const session_mode = FoxRuntime.parseString(p_session_mode);
		const required_features = FoxRuntime.parseString(p_required_features).split(',').map((s) => s.trim()).filter((s) => s !== '');
		const optional_features = FoxRuntime.parseString(p_optional_features).split(',').map((s) => s.trim()).filter((s) => s !== '');
		const requested_reference_space_types = FoxRuntime.parseString(p_requested_reference_spaces).split(',').map((s) => s.trim());
		const onstarted = FoxRuntime.get_func(p_on_session_started);
		const onended = FoxRuntime.get_func(p_on_session_ended);
		const onfailed = FoxRuntime.get_func(p_on_session_failed);
		const oncontroller = FoxRuntime.get_func(p_on_controller_changed);
		const oninputevent = FoxRuntime.get_func(p_on_input_event);
		const onsimpleevent = FoxRuntime.get_func(p_on_simple_event);

		const session_init = {};
		if (required_features.length > 0) {
			session_init['requiredFeatures'] = required_features;
		}
		if (optional_features.length > 0) {
			session_init['optionalFeatures'] = optional_features;
		}

		navigator.xr.requestSession(session_mode, session_init).then(function (session) {
			FoxWebXR.session = session;

			session.addEventListener('end', function (evt) {
				onended();
			});

			session.addEventListener('inputsourceschange', function (evt) {
				let controller_changed = false;
				[evt.added, evt.removed].forEach((lst) => {
					lst.forEach((input_source) => {
						if (input_source.targetRayMode === 'tracked-pointer') {
							controller_changed = true;
						}
					});
				});
				if (controller_changed) {
					oncontroller();
				}
			});

			['selectstart', 'select', 'selectend', 'squeezestart', 'squeeze', 'squeezeend'].forEach((input_event) => {
				session.addEventListener(input_event, function (evt) {
					const c_str = FoxRuntime.allocString(input_event);
					oninputevent(c_str, FoxWebXR.getControllerId(evt.inputSource));
					FoxRuntime.free(c_str);
				});
			});

			session.addEventListener('visibilitychange', function (evt) {
				const c_str = FoxRuntime.allocString('visibility_state_changed');
				onsimpleevent(c_str);
				FoxRuntime.free(c_str);
			});

			const gl_context_handle = _emscripten_webgl_get_current_context(); // eslint-disable-line no-undef
			const gl = GL.getContext(gl_context_handle).GLctx;
			FoxWebXR.gl = gl;

			gl.makeXRCompatible().then(function () {
				session.updateRenderState({
					baseLayer: new XRWebGLLayer(session, gl),
				});

				function onReferenceSpaceSuccess(reference_space, reference_space_type) {
					FoxWebXR.space = reference_space;

					// Using reference_space.addEventListener() crashes when
					// using the polyfill with the WebXR Emulator extension,
					// so we set the event property instead.
					reference_space.onreset = function (evt) {
						const c_str = FoxRuntime.allocString('reference_space_reset');
						onsimpleevent(c_str);
						FoxRuntime.free(c_str);
					};

					// Now that both FoxWebXR.session and FoxWebXR.space are
					// set, we need to pause and resume the main loop for the XR
					// main loop to kick in.
					FoxWebXR.pauseResumeMainLoop();

					// Call in setTimeout() so that errors in the onstarted()
					// callback don't bubble up here and cause Fox to try the
					// next reference space.
					window.setTimeout(function () {
						const c_str = FoxRuntime.allocString(reference_space_type);
						onstarted(c_str);
						FoxRuntime.free(c_str);
					}, 0);
				}

				function requestReferenceSpace() {
					const reference_space_type = requested_reference_space_types.shift();
					session.requestReferenceSpace(reference_space_type)
						.then((refSpace) => {
							onReferenceSpaceSuccess(refSpace, reference_space_type);
						})
						.catch(() => {
							if (requested_reference_space_types.length === 0) {
								const c_str = FoxRuntime.allocString('Unable to get any of the requested reference space types');
								onfailed(c_str);
								FoxRuntime.free(c_str);
							} else {
								requestReferenceSpace();
							}
						});
				}

				requestReferenceSpace();
			}).catch(function (error) {
				const c_str = FoxRuntime.allocString(`Unable to make WebGL context compatible with WebXR: ${error}`);
				onfailed(c_str);
				FoxRuntime.free(c_str);
			});
		}).catch(function (error) {
			const c_str = FoxRuntime.allocString(`Unable to start session: ${error}`);
			onfailed(c_str);
			FoxRuntime.free(c_str);
		});
	},

	Fox_webxr_uninitialize__proxy: 'sync',
	Fox_webxr_uninitialize__sig: 'v',
	Fox_webxr_uninitialize: function () {
		if (FoxWebXR.session) {
			FoxWebXR.session.end()
				// Prevent exception when session has already ended.
				.catch((e) => { });
		}

		// Clean-up the textures we allocated for each view.
		const gl = FoxWebXR.gl;
		for (let i = 0; i < FoxWebXR.textures.length; i++) {
			const texture = FoxWebXR.textures[i];
			if (texture !== null) {
				gl.deleteTexture(texture);
			}
			FoxWebXR.textures[i] = null;

			const texture_id = FoxWebXR.texture_ids[i];
			if (texture_id !== null) {
				GL.textures[texture_id] = null;
			}
			FoxWebXR.texture_ids[i] = null;
		}

		FoxWebXR.session = null;
		FoxWebXR.space = null;
		FoxWebXR.frame = null;
		FoxWebXR.pose = null;

		// Disable the monkey-patched window.requestAnimationFrame() and
		// pause/restart the main loop to activate it on all platforms.
		FoxWebXR.monkeyPatchRequestAnimationFrame(false);
		FoxWebXR.pauseResumeMainLoop();
	},

	Fox_webxr_get_view_count__proxy: 'sync',
	Fox_webxr_get_view_count__sig: 'i',
	Fox_webxr_get_view_count: function () {
		if (!FoxWebXR.session || !FoxWebXR.pose) {
			return 0;
		}
		return FoxWebXR.pose.views.length;
	},

	Fox_webxr_get_render_targetsize__proxy: 'sync',
	Fox_webxr_get_render_targetsize__sig: 'i',
	Fox_webxr_get_render_targetsize: function () {
		if (!FoxWebXR.session || !FoxWebXR.pose) {
			return 0;
		}

		const glLayer = FoxWebXR.session.renderState.baseLayer;
		const view = FoxWebXR.pose.views[0];
		const viewport = glLayer.getViewport(view);

		const buf = FoxRuntime.malloc(2 * 4);
		FoxRuntime.setHeapValue(buf + 0, viewport.width, 'i32');
		FoxRuntime.setHeapValue(buf + 4, viewport.height, 'i32');
		return buf;
	},

	Fox_webxr_get_transform_for_eye__proxy: 'sync',
	Fox_webxr_get_transform_for_eye__sig: 'ii',
	Fox_webxr_get_transform_for_eye: function (p_eye) {
		if (!FoxWebXR.session || !FoxWebXR.pose) {
			return 0;
		}

		const views = FoxWebXR.pose.views;
		let matrix;
		if (p_eye === 0) {
			matrix = FoxWebXR.pose.transform.matrix;
		} else {
			matrix = views[p_eye - 1].transform.matrix;
		}
		const buf = FoxRuntime.malloc(16 * 4);
		for (let i = 0; i < 16; i++) {
			FoxRuntime.setHeapValue(buf + (i * 4), matrix[i], 'float');
		}
		return buf;
	},

	Fox_webxr_get_projection_for_eye__proxy: 'sync',
	Fox_webxr_get_projection_for_eye__sig: 'ii',
	Fox_webxr_get_projection_for_eye: function (p_eye) {
		if (!FoxWebXR.session || !FoxWebXR.pose) {
			return 0;
		}

		const view_index = (p_eye === 2 /* ARVRInterface::EYE_RIGHT */) ? 1 : 0;
		const matrix = FoxWebXR.pose.views[view_index].projectionMatrix;
		const buf = FoxRuntime.malloc(16 * 4);
		for (let i = 0; i < 16; i++) {
			FoxRuntime.setHeapValue(buf + (i * 4), matrix[i], 'float');
		}
		return buf;
	},

	Fox_webxr_get_external_texture_for_eye__proxy: 'sync',
	Fox_webxr_get_external_texture_for_eye__sig: 'ii',
	Fox_webxr_get_external_texture_for_eye: function (p_eye) {
		if (!FoxWebXR.session) {
			return 0;
		}

		const view_index = (p_eye === 2 /* ARVRInterface::EYE_RIGHT */) ? 1 : 0;
		if (FoxWebXR.texture_ids[view_index]) {
			return FoxWebXR.texture_ids[view_index];
		}

		// Check pose separately and after returning the cached texture id,
		// because we won't get a pose in some cases if we lose tracking, and
		// we don't want to return 0 just because tracking was lost.
		if (!FoxWebXR.pose) {
			return 0;
		}

		const glLayer = FoxWebXR.session.renderState.baseLayer;
		const view = FoxWebXR.pose.views[view_index];
		const viewport = glLayer.getViewport(view);
		const gl = FoxWebXR.gl;

		const texture = gl.createTexture();
		gl.bindTexture(gl.TEXTURE_2D, texture);
		gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, viewport.width, viewport.height, 0, gl.RGBA, gl.UNSIGNED_BYTE, null);

		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
		gl.bindTexture(gl.TEXTURE_2D, null);

		const texture_id = GL.getNewId(GL.textures);
		GL.textures[texture_id] = texture;
		FoxWebXR.textures[view_index] = texture;
		FoxWebXR.texture_ids[view_index] = texture_id;
		return texture_id;
	},

	Fox_webxr_commit_for_eye__proxy: 'sync',
	Fox_webxr_commit_for_eye__sig: 'vi',
	Fox_webxr_commit_for_eye: function (p_eye) {
		if (!FoxWebXR.session || !FoxWebXR.pose) {
			return;
		}

		const view_index = (p_eye === 2 /* ARVRInterface::EYE_RIGHT */) ? 1 : 0;
		const glLayer = FoxWebXR.session.renderState.baseLayer;
		const view = FoxWebXR.pose.views[view_index];
		const viewport = glLayer.getViewport(view);
		const gl = FoxWebXR.gl;

		const orig_framebuffer = gl.getParameter(gl.FRAMEBUFFER_BINDING);
		const orig_viewport = gl.getParameter(gl.VIEWPORT);

		// Bind to WebXR's framebuffer.
		gl.bindFramebuffer(gl.FRAMEBUFFER, glLayer.framebuffer);
		gl.viewport(viewport.x, viewport.y, viewport.width, viewport.height);

		FoxWebXR.blitTexture(gl, FoxWebXR.textures[view_index]);

		// Restore state.
		gl.bindFramebuffer(gl.FRAMEBUFFER, orig_framebuffer);
		gl.viewport(orig_viewport[0], orig_viewport[1], orig_viewport[2], orig_viewport[3]);
	},

	Fox_webxr_sample_controller_data__proxy: 'sync',
	Fox_webxr_sample_controller_data__sig: 'v',
	Fox_webxr_sample_controller_data: function () {
		FoxWebXR.sampleControllers();
	},

	Fox_webxr_get_controller_count__proxy: 'sync',
	Fox_webxr_get_controller_count__sig: 'i',
	Fox_webxr_get_controller_count: function () {
		if (!FoxWebXR.session || !FoxWebXR.frame) {
			return 0;
		}
		return FoxWebXR.controllers.length;
	},

	Fox_webxr_is_controller_connected__proxy: 'sync',
	Fox_webxr_is_controller_connected__sig: 'ii',
	Fox_webxr_is_controller_connected: function (p_controller) {
		if (!FoxWebXR.session || !FoxWebXR.frame) {
			return false;
		}
		return !!FoxWebXR.controllers[p_controller];
	},

	Fox_webxr_get_controller_transform__proxy: 'sync',
	Fox_webxr_get_controller_transform__sig: 'ii',
	Fox_webxr_get_controller_transform: function (p_controller) {
		if (!FoxWebXR.session || !FoxWebXR.frame) {
			return 0;
		}

		const controller = FoxWebXR.controllers[p_controller];
		if (!controller) {
			return 0;
		}

		const frame = FoxWebXR.frame;
		const space = FoxWebXR.space;

		const pose = frame.getPose(controller.targetRaySpace, space);
		if (!pose) {
			// This can mean that the controller lost tracking.
			return 0;
		}
		const matrix = pose.transform.matrix;

		const buf = FoxRuntime.malloc(16 * 4);
		for (let i = 0; i < 16; i++) {
			FoxRuntime.setHeapValue(buf + (i * 4), matrix[i], 'float');
		}
		return buf;
	},

	Fox_webxr_get_controller_buttons__proxy: 'sync',
	Fox_webxr_get_controller_buttons__sig: 'ii',
	Fox_webxr_get_controller_buttons: function (p_controller) {
		if (FoxWebXR.controllers.length === 0) {
			return 0;
		}

		const controller = FoxWebXR.controllers[p_controller];
		if (!controller || !controller.gamepad) {
			return 0;
		}

		const button_count = controller.gamepad.buttons.length;

		const buf = FoxRuntime.malloc((button_count + 1) * 4);
		FoxRuntime.setHeapValue(buf, button_count, 'i32');
		for (let i = 0; i < button_count; i++) {
			FoxRuntime.setHeapValue(buf + 4 + (i * 4), controller.gamepad.buttons[i].value, 'float');
		}
		return buf;
	},

	Fox_webxr_get_controller_axes__proxy: 'sync',
	Fox_webxr_get_controller_axes__sig: 'ii',
	Fox_webxr_get_controller_axes: function (p_controller) {
		if (FoxWebXR.controllers.length === 0) {
			return 0;
		}

		const controller = FoxWebXR.controllers[p_controller];
		if (!controller || !controller.gamepad) {
			return 0;
		}

		const axes_count = controller.gamepad.axes.length;

		const buf = FoxRuntime.malloc((axes_count + 1) * 4);
		FoxRuntime.setHeapValue(buf, axes_count, 'i32');
		for (let i = 0; i < axes_count; i++) {
			let value = controller.gamepad.axes[i];
			if (i === 1 || i === 3) {
				// Invert the Y-axis on thumbsticks and trackpads, in order to
				// match OpenXR and other XR platform SDKs.
				value *= -1.0;
			}
			FoxRuntime.setHeapValue(buf + 4 + (i * 4), value, 'float');
		}
		return buf;
	},

	Fox_webxr_get_visibility_state__proxy: 'sync',
	Fox_webxr_get_visibility_state__sig: 'i',
	Fox_webxr_get_visibility_state: function () {
		if (!FoxWebXR.session || !FoxWebXR.session.visibilityState) {
			return 0;
		}

		return FoxRuntime.allocString(FoxWebXR.session.visibilityState);
	},

	Fox_webxr_get_bounds_geometry__proxy: 'sync',
	Fox_webxr_get_bounds_geometry__sig: 'i',
	Fox_webxr_get_bounds_geometry: function () {
		if (!FoxWebXR.space || !FoxWebXR.space.boundsGeometry) {
			return 0;
		}

		const point_count = FoxWebXR.space.boundsGeometry.length;
		if (point_count === 0) {
			return 0;
		}

		const buf = FoxRuntime.malloc(((point_count * 3) + 1) * 4);
		FoxRuntime.setHeapValue(buf, point_count, 'i32');
		for (let i = 0; i < point_count; i++) {
			const point = FoxWebXR.space.boundsGeometry[i];
			FoxRuntime.setHeapValue(buf + ((i * 3) + 1) * 4, point.x, 'float');
			FoxRuntime.setHeapValue(buf + ((i * 3) + 2) * 4, point.y, 'float');
			FoxRuntime.setHeapValue(buf + ((i * 3) + 3) * 4, point.z, 'float');
		}

		return buf;
	},
};

autoAddDeps(FoxWebXR, '$FoxWebXR');
mergeInto(LibraryManager.library, FoxWebXR);
