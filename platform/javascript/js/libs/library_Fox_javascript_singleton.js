/*************************************************************************/
/*  library_Fox_eval.js                                                */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           Fox ENGINE                                */
/*                      https://Foxengine.org                          */
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

const FoxJSWrapper = {

	$FoxJSWrapper__deps: ['$FoxRuntime', '$IDHandler'],
	$FoxJSWrapper__postset: 'FoxJSWrapper.proxies = new Map();',
	$FoxJSWrapper: {
		proxies: null,
		cb_ret: null,

		MyProxy: function (val) {
			const id = IDHandler.add(this);
			FoxJSWrapper.proxies.set(val, id);
			let refs = 1;
			this.ref = function () {
				refs++;
			};
			this.unref = function () {
				refs--;
				if (refs === 0) {
					IDHandler.remove(id);
					FoxJSWrapper.proxies.delete(val);
				}
			};
			this.get_val = function () {
				return val;
			};
			this.get_id = function () {
				return id;
			};
		},

		get_proxied: function (val) {
			const id = FoxJSWrapper.proxies.get(val);
			if (id === undefined) {
				const proxy = new FoxJSWrapper.MyProxy(val);
				return proxy.get_id();
			}
			IDHandler.get(id).ref();
			return id;
		},

		get_proxied_value: function (id) {
			const proxy = IDHandler.get(id);
			if (proxy === undefined) {
				return undefined;
			}
			return proxy.get_val();
		},

		variant2js: function (type, val) {
			switch (type) {
			case 0:
				return null;
			case 1:
				return !!FoxRuntime.getHeapValue(val, 'i64');
			case 2:
				return FoxRuntime.getHeapValue(val, 'i64');
			case 3:
				return FoxRuntime.getHeapValue(val, 'double');
			case 4:
				return FoxRuntime.parseString(FoxRuntime.getHeapValue(val, '*'));
			case 21: // OBJECT
				return FoxJSWrapper.get_proxied_value(FoxRuntime.getHeapValue(val, 'i64'));
			default:
				return undefined;
			}
		},

		js2variant: function (p_val, p_exchange) {
			if (p_val === undefined || p_val === null) {
				return 0; // NIL
			}
			const type = typeof (p_val);
			if (type === 'boolean') {
				FoxRuntime.setHeapValue(p_exchange, p_val, 'i64');
				return 1; // BOOL
			} else if (type === 'number') {
				if (Number.isInteger(p_val)) {
					FoxRuntime.setHeapValue(p_exchange, p_val, 'i64');
					return 2; // INT
				}
				FoxRuntime.setHeapValue(p_exchange, p_val, 'double');
				return 3; // REAL
			} else if (type === 'string') {
				const c_str = FoxRuntime.allocString(p_val);
				FoxRuntime.setHeapValue(p_exchange, c_str, '*');
				return 4; // STRING
			}
			const id = FoxJSWrapper.get_proxied(p_val);
			FoxRuntime.setHeapValue(p_exchange, id, 'i64');
			return 21;
		},
	},

	Fox_js_wrapper_interface_get__sig: 'ii',
	Fox_js_wrapper_interface_get: function (p_name) {
		const name = FoxRuntime.parseString(p_name);
		if (typeof (window[name]) !== 'undefined') {
			return FoxJSWrapper.get_proxied(window[name]);
		}
		return 0;
	},

	Fox_js_wrapper_object_get__sig: 'iiii',
	Fox_js_wrapper_object_get: function (p_id, p_exchange, p_prop) {
		const obj = FoxJSWrapper.get_proxied_value(p_id);
		if (obj === undefined) {
			return 0;
		}
		if (p_prop) {
			const prop = FoxRuntime.parseString(p_prop);
			try {
				return FoxJSWrapper.js2variant(obj[prop], p_exchange);
			} catch (e) {
				FoxRuntime.error(`Error getting variable ${prop} on object`, obj);
				return 0; // NIL
			}
		}
		return FoxJSWrapper.js2variant(obj, p_exchange);
	},

	Fox_js_wrapper_object_set__sig: 'viiii',
	Fox_js_wrapper_object_set: function (p_id, p_name, p_type, p_exchange) {
		const obj = FoxJSWrapper.get_proxied_value(p_id);
		if (obj === undefined) {
			return;
		}
		const name = FoxRuntime.parseString(p_name);
		try {
			obj[name] = FoxJSWrapper.variant2js(p_type, p_exchange);
		} catch (e) {
			FoxRuntime.error(`Error setting variable ${name} on object`, obj);
		}
	},

	Fox_js_wrapper_object_call__sig: 'iiiiiiiii',
	Fox_js_wrapper_object_call: function (p_id, p_method, p_args, p_argc, p_convert_callback, p_exchange, p_lock, p_free_lock_callback) {
		const obj = FoxJSWrapper.get_proxied_value(p_id);
		if (obj === undefined) {
			return -1;
		}
		const method = FoxRuntime.parseString(p_method);
		const convert = FoxRuntime.get_func(p_convert_callback);
		const freeLock = FoxRuntime.get_func(p_free_lock_callback);
		const args = new Array(p_argc);
		for (let i = 0; i < p_argc; i++) {
			const type = convert(p_args, i, p_exchange, p_lock);
			const lock = FoxRuntime.getHeapValue(p_lock, '*');
			args[i] = FoxJSWrapper.variant2js(type, p_exchange);
			if (lock) {
				freeLock(p_lock, type);
			}
		}
		try {
			const res = obj[method](...args);
			return FoxJSWrapper.js2variant(res, p_exchange);
		} catch (e) {
			FoxRuntime.error(`Error calling method ${method} on:`, obj, 'error:', e);
			return -1;
		}
	},

	Fox_js_wrapper_object_unref__sig: 'vi',
	Fox_js_wrapper_object_unref: function (p_id) {
		const proxy = IDHandler.get(p_id);
		if (proxy !== undefined) {
			proxy.unref();
		}
	},

	Fox_js_wrapper_create_cb__sig: 'iii',
	Fox_js_wrapper_create_cb: function (p_ref, p_func) {
		const func = FoxRuntime.get_func(p_func);
		let id = 0;
		const cb = function () {
			if (!FoxJSWrapper.get_proxied_value(id)) {
				return undefined;
			}
			// The callback will store the returned value in this variable via
			// "Fox_js_wrapper_object_set_cb_ret" upon calling the user function.
			// This is safe! JavaScript is single threaded (and using it in threads is not a good idea anyway).
			FoxJSWrapper.cb_ret = null;
			const args = Array.from(arguments);
			func(p_ref, FoxJSWrapper.get_proxied(args), args.length);
			const ret = FoxJSWrapper.cb_ret;
			FoxJSWrapper.cb_ret = null;
			return ret;
		};
		id = FoxJSWrapper.get_proxied(cb);
		return id;
	},

	Fox_js_wrapper_object_set_cb_ret__sig: 'vii',
	Fox_js_wrapper_object_set_cb_ret: function (p_val_type, p_val_ex) {
		FoxJSWrapper.cb_ret = FoxJSWrapper.variant2js(p_val_type, p_val_ex);
	},

	Fox_js_wrapper_object_getvar__sig: 'iiii',
	Fox_js_wrapper_object_getvar: function (p_id, p_type, p_exchange) {
		const obj = FoxJSWrapper.get_proxied_value(p_id);
		if (obj === undefined) {
			return -1;
		}
		const prop = FoxJSWrapper.variant2js(p_type, p_exchange);
		if (prop === undefined || prop === null) {
			return -1;
		}
		try {
			return FoxJSWrapper.js2variant(obj[prop], p_exchange);
		} catch (e) {
			FoxRuntime.error(`Error getting variable ${prop} on object`, obj, e);
			return -1;
		}
	},

	Fox_js_wrapper_object_setvar__sig: 'iiiiii',
	Fox_js_wrapper_object_setvar: function (p_id, p_key_type, p_key_ex, p_val_type, p_val_ex) {
		const obj = FoxJSWrapper.get_proxied_value(p_id);
		if (obj === undefined) {
			return -1;
		}
		const key = FoxJSWrapper.variant2js(p_key_type, p_key_ex);
		try {
			obj[key] = FoxJSWrapper.variant2js(p_val_type, p_val_ex);
			return 0;
		} catch (e) {
			FoxRuntime.error(`Error setting variable ${key} on object`, obj);
			return -1;
		}
	},

	Fox_js_wrapper_create_object__sig: 'iiiiiiii',
	Fox_js_wrapper_create_object: function (p_object, p_args, p_argc, p_convert_callback, p_exchange, p_lock, p_free_lock_callback) {
		const name = FoxRuntime.parseString(p_object);
		if (typeof (window[name]) === 'undefined') {
			return -1;
		}
		const convert = FoxRuntime.get_func(p_convert_callback);
		const freeLock = FoxRuntime.get_func(p_free_lock_callback);
		const args = new Array(p_argc);
		for (let i = 0; i < p_argc; i++) {
			const type = convert(p_args, i, p_exchange, p_lock);
			const lock = FoxRuntime.getHeapValue(p_lock, '*');
			args[i] = FoxJSWrapper.variant2js(type, p_exchange);
			if (lock) {
				freeLock(p_lock, type);
			}
		}
		try {
			const res = new window[name](...args);
			return FoxJSWrapper.js2variant(res, p_exchange);
		} catch (e) {
			FoxRuntime.error(`Error calling constructor ${name} with args:`, args, 'error:', e);
			return -1;
		}
	},
};

autoAddDeps(FoxJSWrapper, '$FoxJSWrapper');
mergeInto(LibraryManager.library, FoxJSWrapper);

const FoxEval = {
	Fox_js_eval__deps: ['$FoxRuntime'],
	Fox_js_eval__sig: 'iiiiiii',
	Fox_js_eval: function (p_js, p_use_global_ctx, p_union_ptr, p_byte_arr, p_byte_arr_write, p_callback) {
		const js_code = FoxRuntime.parseString(p_js);
		let eval_ret = null;
		try {
			if (p_use_global_ctx) {
				// indirect eval call grants global execution context
				const global_eval = eval; // eslint-disable-line no-eval
				eval_ret = global_eval(js_code);
			} else {
				eval_ret = eval(js_code); // eslint-disable-line no-eval
			}
		} catch (e) {
			FoxRuntime.error(e);
		}

		switch (typeof eval_ret) {
		case 'boolean':
			FoxRuntime.setHeapValue(p_union_ptr, eval_ret, 'i32');
			return 1; // BOOL

		case 'number':
			FoxRuntime.setHeapValue(p_union_ptr, eval_ret, 'double');
			return 3; // REAL

		case 'string':
			FoxRuntime.setHeapValue(p_union_ptr, FoxRuntime.allocString(eval_ret), '*');
			return 4; // STRING

		case 'object':
			if (eval_ret === null) {
				break;
			}

			if (ArrayBuffer.isView(eval_ret) && !(eval_ret instanceof Uint8Array)) {
				eval_ret = new Uint8Array(eval_ret.buffer);
			} else if (eval_ret instanceof ArrayBuffer) {
				eval_ret = new Uint8Array(eval_ret);
			}
			if (eval_ret instanceof Uint8Array) {
				const func = FoxRuntime.get_func(p_callback);
				const bytes_ptr = func(p_byte_arr, p_byte_arr_write, eval_ret.length);
				HEAPU8.set(eval_ret, bytes_ptr);
				return 20; // POOL_BYTE_ARRAY
			}
			break;

			// no default
		}
		return 0; // NIL
	},
};

mergeInto(LibraryManager.library, FoxEval);
