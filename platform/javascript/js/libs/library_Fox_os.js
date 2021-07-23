/*************************************************************************/
/*  library_Fox_os.js                                                  */
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

const IDHandler = {
	$IDHandler: {
		_last_id: 0,
		_references: {},

		get: function (p_id) {
			return IDHandler._references[p_id];
		},

		add: function (p_data) {
			const id = ++IDHandler._last_id;
			IDHandler._references[id] = p_data;
			return id;
		},

		remove: function (p_id) {
			delete IDHandler._references[p_id];
		},
	},
};

autoAddDeps(IDHandler, '$IDHandler');
mergeInto(LibraryManager.library, IDHandler);

const FoxConfig = {
	$FoxConfig__postset: 'Module["initConfig"] = FoxConfig.init_config;',
	$FoxConfig__deps: ['$FoxRuntime'],
	$FoxConfig: {
		canvas: null,
		locale: 'en',
		canvas_resize_policy: 2, // Adaptive
		virtual_keyboard: false,
		persistent_drops: false,
		on_execute: null,
		on_exit: null,

		init_config: function (p_opts) {
			FoxConfig.canvas_resize_policy = p_opts['canvasResizePolicy'];
			FoxConfig.canvas = p_opts['canvas'];
			FoxConfig.locale = p_opts['locale'] || FoxConfig.locale;
			FoxConfig.virtual_keyboard = p_opts['virtualKeyboard'];
			FoxConfig.persistent_drops = !!p_opts['persistentDrops'];
			FoxConfig.on_execute = p_opts['onExecute'];
			FoxConfig.on_exit = p_opts['onExit'];
			if (p_opts['focusCanvas']) {
				FoxConfig.canvas.focus();
			}
		},

		locate_file: function (file) {
			return Module['locateFile'](file); // eslint-disable-line no-undef
		},
		clear: function () {
			FoxConfig.canvas = null;
			FoxConfig.locale = 'en';
			FoxConfig.canvas_resize_policy = 2;
			FoxConfig.virtual_keyboard = false;
			FoxConfig.persistent_drops = false;
			FoxConfig.on_execute = null;
			FoxConfig.on_exit = null;
		},
	},

	Fox_js_config_canvas_id_get__sig: 'vii',
	Fox_js_config_canvas_id_get: function (p_ptr, p_ptr_max) {
		FoxRuntime.stringToHeap(`#${FoxConfig.canvas.id}`, p_ptr, p_ptr_max);
	},

	Fox_js_config_locale_get__sig: 'vii',
	Fox_js_config_locale_get: function (p_ptr, p_ptr_max) {
		FoxRuntime.stringToHeap(FoxConfig.locale, p_ptr, p_ptr_max);
	},
};

autoAddDeps(FoxConfig, '$FoxConfig');
mergeInto(LibraryManager.library, FoxConfig);

const FoxFS = {
	$FoxFS__deps: ['$FS', '$IDBFS', '$FoxRuntime'],
	$FoxFS__postset: [
		'Module["initFS"] = FoxFS.init;',
		'Module["copyToFS"] = FoxFS.copy_to_fs;',
	].join(''),
	$FoxFS: {
		_idbfs: false,
		_syncing: false,
		_mount_points: [],

		is_persistent: function () {
			return FoxFS._idbfs ? 1 : 0;
		},

		// Initialize Fox file system, setting up persistent paths.
		// Returns a promise that resolves when the FS is ready.
		// We keep track of mount_points, so that we can properly close the IDBFS
		// since emscripten is not doing it by itself. (emscripten GH#12516).
		init: function (persistentPaths) {
			FoxFS._idbfs = false;
			if (!Array.isArray(persistentPaths)) {
				return Promise.reject(new Error('Persistent paths must be an array'));
			}
			if (!persistentPaths.length) {
				return Promise.resolve();
			}
			FoxFS._mount_points = persistentPaths.slice();

			function createRecursive(dir) {
				try {
					FS.stat(dir);
				} catch (e) {
					if (e.errno !== ERRNO_CODES.ENOENT) {
						throw e;
					}
					FS.mkdirTree(dir);
				}
			}

			FoxFS._mount_points.forEach(function (path) {
				createRecursive(path);
				FS.mount(IDBFS, {}, path);
			});
			return new Promise(function (resolve, reject) {
				FS.syncfs(true, function (err) {
					if (err) {
						FoxFS._mount_points = [];
						FoxFS._idbfs = false;
						FoxRuntime.print(`IndexedDB not available: ${err.message}`);
					} else {
						FoxFS._idbfs = true;
					}
					resolve(err);
				});
			});
		},

		// Deinit Fox file system, making sure to unmount file systems, and close IDBFS(s).
		deinit: function () {
			FoxFS._mount_points.forEach(function (path) {
				try {
					FS.unmount(path);
				} catch (e) {
					FoxRuntime.print('Already unmounted', e);
				}
				if (FoxFS._idbfs && IDBFS.dbs[path]) {
					IDBFS.dbs[path].close();
					delete IDBFS.dbs[path];
				}
			});
			FoxFS._mount_points = [];
			FoxFS._idbfs = false;
			FoxFS._syncing = false;
		},

		sync: function () {
			if (FoxFS._syncing) {
				FoxRuntime.error('Already syncing!');
				return Promise.resolve();
			}
			FoxFS._syncing = true;
			return new Promise(function (resolve, reject) {
				FS.syncfs(false, function (error) {
					if (error) {
						FoxRuntime.error(`Failed to save IDB file system: ${error.message}`);
					}
					FoxFS._syncing = false;
					resolve(error);
				});
			});
		},

		// Copies a buffer to the internal file system. Creating directories recursively.
		copy_to_fs: function (path, buffer) {
			const idx = path.lastIndexOf('/');
			let dir = '/';
			if (idx > 0) {
				dir = path.slice(0, idx);
			}
			try {
				FS.stat(dir);
			} catch (e) {
				if (e.errno !== ERRNO_CODES.ENOENT) {
					throw e;
				}
				FS.mkdirTree(dir);
			}
			FS.writeFile(path, new Uint8Array(buffer));
		},
	},
};
mergeInto(LibraryManager.library, FoxFS);

const FoxOS = {
	$FoxOS__deps: ['$FoxRuntime', '$FoxConfig', '$FoxFS'],
	$FoxOS__postset: [
		'Module["request_quit"] = function() { FoxOS.request_quit() };',
		'Module["onExit"] = FoxOS.cleanup;',
		'FoxOS._fs_sync_promise = Promise.resolve();',
	].join(''),
	$FoxOS: {
		request_quit: function () {},
		_async_cbs: [],
		_fs_sync_promise: null,

		atexit: function (p_promise_cb) {
			FoxOS._async_cbs.push(p_promise_cb);
		},

		cleanup: function (exit_code) {
			const cb = FoxConfig.on_exit;
			FoxFS.deinit();
			FoxConfig.clear();
			if (cb) {
				cb(exit_code);
			}
		},

		finish_async: function (callback) {
			FoxOS._fs_sync_promise.then(function (err) {
				const promises = [];
				FoxOS._async_cbs.forEach(function (cb) {
					promises.push(new Promise(cb));
				});
				return Promise.all(promises);
			}).then(function () {
				return FoxFS.sync(); // Final FS sync.
			}).then(function (err) {
				// Always deferred.
				setTimeout(function () {
					callback();
				}, 0);
			});
		},
	},

	Fox_js_os_finish_async__sig: 'vi',
	Fox_js_os_finish_async: function (p_callback) {
		const func = FoxRuntime.get_func(p_callback);
		FoxOS.finish_async(func);
	},

	Fox_js_os_request_quit_cb__sig: 'vi',
	Fox_js_os_request_quit_cb: function (p_callback) {
		FoxOS.request_quit = FoxRuntime.get_func(p_callback);
	},

	Fox_js_os_fs_is_persistent__sig: 'i',
	Fox_js_os_fs_is_persistent: function () {
		return FoxFS.is_persistent();
	},

	Fox_js_os_fs_sync__sig: 'vi',
	Fox_js_os_fs_sync: function (callback) {
		const func = FoxRuntime.get_func(callback);
		FoxOS._fs_sync_promise = FoxFS.sync();
		FoxOS._fs_sync_promise.then(function (err) {
			func();
		});
	},

	Fox_js_os_execute__sig: 'ii',
	Fox_js_os_execute: function (p_json) {
		const json_args = FoxRuntime.parseString(p_json);
		const args = JSON.parse(json_args);
		if (FoxConfig.on_execute) {
			FoxConfig.on_execute(args);
			return 0;
		}
		return 1;
	},

	Fox_js_os_shell_open__sig: 'vi',
	Fox_js_os_shell_open: function (p_uri) {
		window.open(FoxRuntime.parseString(p_uri), '_blank');
	},

	Fox_js_os_hw_concurrency_get__sig: 'i',
	Fox_js_os_hw_concurrency_get: function () {
		return navigator.hardwareConcurrency || 1;
	},

	Fox_js_os_download_buffer__sig: 'viiii',
	Fox_js_os_download_buffer: function (p_ptr, p_size, p_name, p_mime) {
		const buf = FoxRuntime.heapSlice(HEAP8, p_ptr, p_size);
		const name = FoxRuntime.parseString(p_name);
		const mime = FoxRuntime.parseString(p_mime);
		const blob = new Blob([buf], { type: mime });
		const url = window.URL.createObjectURL(blob);
		const a = document.createElement('a');
		a.href = url;
		a.download = name;
		a.style.display = 'none';
		document.body.appendChild(a);
		a.click();
		a.remove();
		window.URL.revokeObjectURL(url);
	},
};

autoAddDeps(FoxOS, '$FoxOS');
mergeInto(LibraryManager.library, FoxOS);
