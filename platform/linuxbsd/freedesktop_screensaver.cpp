/*************************************************************************/
/*  freedesktop_screensaver.cpp                                          */
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

#include "freedesktop_screensaver.h"

#ifdef DBUS_ENABLED

#include "core/config/project_settings.h"

#include <dbus/dbus.h>

#define BUS_OBJECT_NAME "org.freedesktop.ScreenSaver"
#define BUS_OBJECT_PATH "/org/freedesktop/ScreenSaver"
#define BUS_INTERFACE "org.freedesktop.ScreenSaver"

void FreeDesktopScreenSaver::inhibit() {
	if (unsupported) {
		return;
	}

	DBusError error;
	dbus_error_init(&error);

	DBusConnection *bus = dbus_bus_get(DBUS_BUS_SESSION, &error);
	if (dbus_error_is_set(&error)) {
		unsupported = true;
		return;
	}

	String app_name_string = ProjectSettings::get_singleton()->get("application/config/name");
	CharString app_name_utf8 = app_name_string.utf8();
	const char *app_name = app_name_string.is_empty() ? "Fox Engine" : app_name_utf8.get_data();

	const char *reason = "Running Fox Engine project";

	DBusMessage *message = dbus_message_new_method_call(
			BUS_OBJECT_NAME, BUS_OBJECT_PATH, BUS_INTERFACE,
			"Inhibit");
	dbus_message_append_args(
			message,
			DBUS_TYPE_STRING, &app_name,
			DBUS_TYPE_STRING, &reason,
			DBUS_TYPE_INVALID);

	DBusMessage *reply = dbus_connection_send_with_reply_and_block(bus, message, 50, &error);
	dbus_message_unref(message);
	if (dbus_error_is_set(&error)) {
		dbus_connection_unref(bus);
		unsupported = false;
		return;
	}

	DBusMessageIter reply_iter;
	dbus_message_iter_init(reply, &reply_iter);
	dbus_message_iter_get_basic(&reply_iter, &cookie);
	print_verbose("FreeDesktopScreenSaver: Acquired screensaver inhibition cookie: " + uitos(cookie));

	dbus_message_unref(reply);
	dbus_connection_unref(bus);
}

void FreeDesktopScreenSaver::uninhibit() {
	if (unsupported) {
		return;
	}

	DBusError error;
	dbus_error_init(&error);

	DBusConnection *bus = dbus_bus_get(DBUS_BUS_SESSION, &error);
	if (dbus_error_is_set(&error)) {
		unsupported = true;
		return;
	}

	DBusMessage *message = dbus_message_new_method_call(
			BUS_OBJECT_NAME, BUS_OBJECT_PATH, BUS_INTERFACE,
			"UnInhibit");
	dbus_message_append_args(
			message,
			DBUS_TYPE_UINT32, &cookie,
			DBUS_TYPE_INVALID);

	DBusMessage *reply = dbus_connection_send_with_reply_and_block(bus, message, 50, &error);
	if (dbus_error_is_set(&error)) {
		dbus_connection_unref(bus);
		unsupported = true;
		return;
	}

	print_verbose("FreeDesktopScreenSaver: Released screensaver inhibition cookie: " + uitos(cookie));

	dbus_message_unref(message);
	dbus_message_unref(reply);
	dbus_connection_unref(bus);
}

#endif // DBUS_ENABLED
