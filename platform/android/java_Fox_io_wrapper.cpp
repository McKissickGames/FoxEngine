/*************************************************************************/
/*  java_Fox_io_wrapper.cpp                                            */
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

#include "java_Fox_io_wrapper.h"
#include "core/error/error_list.h"

// JNIEnv is only valid within the thread it belongs to, in a multi threading environment
// we can't cache it.
// For FoxIO we call all access methods from our thread and we thus get a valid JNIEnv
// from get_jni_env().

FoxIOJavaWrapper::FoxIOJavaWrapper(JNIEnv *p_env, jobject p_Fox_io_instance) {
	Fox_io_instance = p_env->NewGlobalRef(p_Fox_io_instance);
	if (Fox_io_instance) {
		cls = p_env->GetObjectClass(Fox_io_instance);
		if (cls) {
			cls = (jclass)p_env->NewGlobalRef(cls);
		} else {
			// this is a pretty serious fail.. bail... pointers will stay 0
			return;
		}

		_open_URI = p_env->GetMethodID(cls, "openURI", "(Ljava/lang/String;)I");
		_get_data_dir = p_env->GetMethodID(cls, "getDataDir", "()Ljava/lang/String;");
		_get_external_data_dir = p_env->GetMethodID(cls, "getExternalDataDir", "()Ljava/lang/String;");
		_get_locale = p_env->GetMethodID(cls, "getLocale", "()Ljava/lang/String;");
		_get_model = p_env->GetMethodID(cls, "getModel", "()Ljava/lang/String;");
		_get_screen_DPI = p_env->GetMethodID(cls, "getScreenDPI", "()I");
		_screen_get_usable_rect = p_env->GetMethodID(cls, "screenGetUsableRect", "()[I"),
		_get_unique_id = p_env->GetMethodID(cls, "getUniqueID", "()Ljava/lang/String;");
		_show_keyboard = p_env->GetMethodID(cls, "showKeyboard", "(Ljava/lang/String;ZIII)V");
		_hide_keyboard = p_env->GetMethodID(cls, "hideKeyboard", "()V");
		_set_screen_orientation = p_env->GetMethodID(cls, "setScreenOrientation", "(I)V");
		_get_screen_orientation = p_env->GetMethodID(cls, "getScreenOrientation", "()I");
		_get_system_dir = p_env->GetMethodID(cls, "getSystemDir", "(I)Ljava/lang/String;");
	}
}

FoxIOJavaWrapper::~FoxIOJavaWrapper() {
	// nothing to do here for now
}

jobject FoxIOJavaWrapper::get_instance() {
	return Fox_io_instance;
}

Error FoxIOJavaWrapper::open_uri(const String &p_uri) {
	if (_open_URI) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, ERR_UNAVAILABLE);
		jstring jStr = env->NewStringUTF(p_uri.utf8().get_data());
		return env->CallIntMethod(Fox_io_instance, _open_URI, jStr) ? ERR_CANT_OPEN : OK;
	} else {
		return ERR_UNAVAILABLE;
	}
}

String FoxIOJavaWrapper::get_user_data_dir() {
	if (_get_data_dir) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, String());
		jstring s = (jstring)env->CallObjectMethod(Fox_io_instance, _get_data_dir);
		return jstring_to_string(s, env);
	} else {
		return String();
	}
}

String FoxIOJavaWrapper::get_external_data_dir() {
	if (_get_external_data_dir) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, String());
		jstring s = (jstring)env->CallObjectMethod(Fox_io_instance, _get_external_data_dir);
		return jstring_to_string(s, env);
	} else {
		return String();
	}
}

String FoxIOJavaWrapper::get_locale() {
	if (_get_locale) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, String());
		jstring s = (jstring)env->CallObjectMethod(Fox_io_instance, _get_locale);
		return jstring_to_string(s, env);
	} else {
		return String();
	}
}

String FoxIOJavaWrapper::get_model() {
	if (_get_model) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, String());
		jstring s = (jstring)env->CallObjectMethod(Fox_io_instance, _get_model);
		return jstring_to_string(s, env);
	} else {
		return String();
	}
}

int FoxIOJavaWrapper::get_screen_dpi() {
	if (_get_screen_DPI) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, 160);
		return env->CallIntMethod(Fox_io_instance, _get_screen_DPI);
	} else {
		return 160;
	}
}

void FoxIOJavaWrapper::screen_get_usable_rect(int (&p_rect_xywh)[4]) {
	if (_screen_get_usable_rect) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND(env == nullptr);
		jintArray returnArray = (jintArray)env->CallObjectMethod(Fox_io_instance, _screen_get_usable_rect);
		ERR_FAIL_COND(env->GetArrayLength(returnArray) != 4);
		jint *arrayBody = env->GetIntArrayElements(returnArray, JNI_FALSE);
		for (int i = 0; i < 4; i++) {
			p_rect_xywh[i] = arrayBody[i];
		}
		env->ReleaseIntArrayElements(returnArray, arrayBody, 0);
	}
}

String FoxIOJavaWrapper::get_unique_id() {
	if (_get_unique_id) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, String());
		jstring s = (jstring)env->CallObjectMethod(Fox_io_instance, _get_unique_id);
		return jstring_to_string(s, env);
	} else {
		return String();
	}
}

bool FoxIOJavaWrapper::has_vk() {
	return (_show_keyboard != 0) && (_hide_keyboard != 0);
}

void FoxIOJavaWrapper::show_vk(const String &p_existing, bool p_multiline, int p_max_input_length, int p_cursor_start, int p_cursor_end) {
	if (_show_keyboard) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND(env == nullptr);
		jstring jStr = env->NewStringUTF(p_existing.utf8().get_data());
		env->CallVoidMethod(Fox_io_instance, _show_keyboard, jStr, p_multiline, p_max_input_length, p_cursor_start, p_cursor_end);
	}
}

void FoxIOJavaWrapper::hide_vk() {
	if (_hide_keyboard) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND(env == nullptr);
		env->CallVoidMethod(Fox_io_instance, _hide_keyboard);
	}
}

void FoxIOJavaWrapper::set_screen_orientation(int p_orient) {
	if (_set_screen_orientation) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND(env == nullptr);
		env->CallVoidMethod(Fox_io_instance, _set_screen_orientation, p_orient);
	}
}

int FoxIOJavaWrapper::get_screen_orientation() {
	if (_get_screen_orientation) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, 0);
		return env->CallIntMethod(Fox_io_instance, _get_screen_orientation);
	} else {
		return 0;
	}
}

String FoxIOJavaWrapper::get_system_dir(int p_dir) {
	if (_get_system_dir) {
		JNIEnv *env = get_jni_env();
		ERR_FAIL_COND_V(env == nullptr, String("."));
		jstring s = (jstring)env->CallObjectMethod(Fox_io_instance, _get_system_dir, p_dir);
		return jstring_to_string(s, env);
	} else {
		return String(".");
	}
}

// SafeNumeric because it can be changed from non-main thread and we need to
// ensure the change is immediately visible to other threads.
static SafeNumeric<int> virtual_keyboard_height;

int FoxIOJavaWrapper::get_vk_height() {
	return virtual_keyboard_height.get();
}

void FoxIOJavaWrapper::set_vk_height(int p_height) {
	virtual_keyboard_height.set(p_height);
}
