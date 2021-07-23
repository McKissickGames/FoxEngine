/*************************************************************************/
/*  Fox_webxr.h                                                        */
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

#ifndef Fox_WEBXR_H
#define Fox_WEBXR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"

typedef void (*FoxWebXRSupportedCallback)(char *p_session_mode, int p_supported);
typedef void (*FoxWebXRStartedCallback)(char *p_reference_space_type);
typedef void (*FoxWebXREndedCallback)();
typedef void (*FoxWebXRFailedCallback)(char *p_message);
typedef void (*FoxWebXRControllerCallback)();
typedef void (*FoxWebXRInputEventCallback)(char *p_signal_name, int p_controller_id);
typedef void (*FoxWebXRSimpleEventCallback)(char *p_signal_name);

extern int Fox_webxr_is_supported();
extern void Fox_webxr_is_session_supported(const char *p_session_mode, FoxWebXRSupportedCallback p_callback);

extern void Fox_webxr_initialize(
		const char *p_session_mode,
		const char *p_required_features,
		const char *p_optional_features,
		const char *p_requested_reference_space_types,
		FoxWebXRStartedCallback p_on_session_started,
		FoxWebXREndedCallback p_on_session_ended,
		FoxWebXRFailedCallback p_on_session_failed,
		FoxWebXRControllerCallback p_on_controller_changed,
		FoxWebXRInputEventCallback p_on_input_event,
		FoxWebXRSimpleEventCallback p_on_simple_event);
extern void Fox_webxr_uninitialize();

extern int Fox_webxr_get_view_count();
extern int *Fox_webxr_get_render_targetsize();
extern float *Fox_webxr_get_transform_for_eye(int p_eye);
extern float *Fox_webxr_get_projection_for_eye(int p_eye);
extern int Fox_webxr_get_external_texture_for_eye(int p_eye);
extern void Fox_webxr_commit_for_eye(int p_eye);

extern void Fox_webxr_sample_controller_data();
extern int Fox_webxr_get_controller_count();
extern int Fox_webxr_is_controller_connected(int p_controller);
extern float *Fox_webxr_get_controller_transform(int p_controller);
extern int *Fox_webxr_get_controller_buttons(int p_controller);
extern int *Fox_webxr_get_controller_axes(int p_controller);

extern char *Fox_webxr_get_visibility_state();
extern int *Fox_webxr_get_bounds_geometry();

#ifdef __cplusplus
}
#endif

#endif /* Fox_WEBXR_H */
