/*************************************************************************/
/*  java_Fox_io_wrapper.h                                              */
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

// note, swapped java and Fox around in the file name so all the java
// wrappers are together

#ifndef JAVA_Fox_IO_WRAPPER_H
#define JAVA_Fox_IO_WRAPPER_H

#include <android/log.h>
#include <jni.h>

#include "string_android.h"

// Class that makes functions in java/src/org/Foxengine/Fox/FoxIO.java callable from C++
class FoxIOJavaWrapper {
private:
	jobject Fox_io_instance;
	jclass cls;

	jmethodID _open_URI = 0;
	jmethodID _get_data_dir = 0;
	jmethodID _get_external_data_dir = 0;
	jmethodID _get_locale = 0;
	jmethodID _get_model = 0;
	jmethodID _get_screen_DPI = 0;
	jmethodID _screen_get_usable_rect = 0;
	jmethodID _get_unique_id = 0;
	jmethodID _show_keyboard = 0;
	jmethodID _hide_keyboard = 0;
	jmethodID _set_screen_orientation = 0;
	jmethodID _get_screen_orientation = 0;
	jmethodID _get_system_dir = 0;

public:
	FoxIOJavaWrapper(JNIEnv *p_env, jobject p_Fox_io_instance);
	~FoxIOJavaWrapper();

	jobject get_instance();

	Error open_uri(const String &p_uri);
	String get_user_data_dir();
	String get_external_data_dir();
	String get_locale();
	String get_model();
	int get_screen_dpi();
	void screen_get_usable_rect(int (&p_rect_xywh)[4]);
	String get_unique_id();
	bool has_vk();
	void show_vk(const String &p_existing, bool p_multiline, int p_max_input_length, int p_cursor_start, int p_cursor_end);
	void hide_vk();
	int get_vk_height();
	void set_vk_height(int p_height);
	void set_screen_orientation(int p_orient);
	int get_screen_orientation();
	String get_system_dir(int p_dir);
};

#endif /* !JAVA_Fox_IO_WRAPPER_H */
