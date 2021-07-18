/*************************************************************************/
/*  Fox_xr.h                                                           */
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

#ifndef Fox_NATIVEXR_H
#define Fox_NATIVEXR_H

#include <gdnative/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

// For future versions of the API we should only add new functions at the end of the structure and use the
// version info to detect whether a call is available

// Use these to populate version in your plugin
#define FoxVR_API_MAJOR 4
#define FoxVR_API_MINOR 0

typedef struct {
	Fox_gdnative_api_version version; /* version of our API */
	void *(*constructor)(Fox_object *);
	void (*destructor)(void *);
	Fox_string (*get_name)(const void *);
	Fox_int (*get_capabilities)(const void *);
	Fox_bool (*get_anchor_detection_is_enabled)(const void *);
	void (*set_anchor_detection_is_enabled)(void *, Fox_bool);
	Fox_int (*get_view_count)(const void *);
	Fox_bool (*is_initialized)(const void *);
	Fox_bool (*initialize)(void *);
	void (*uninitialize)(void *);
	Fox_vector2 (*get_render_targetsize)(const void *);

	Fox_transform3d (*get_camera_transform)(void *);
	Fox_transform3d (*get_transform_for_view)(void *, Fox_int, Fox_transform3d *);
	void (*fill_projection_for_view)(void *, Fox_real_t *, Fox_int, Fox_real_t, Fox_real_t, Fox_real_t);
	void (*commit_views)(void *, Fox_rid *, Fox_rect2 *);

	void (*process)(void *);
	void (*notification)(void *, Fox_int);
	Fox_int (*get_camera_feed_id)(void *);

	// possibly deprecate but adding/keeping as a reminder these are in Fox 3
	void (*commit_for_eye)(void *, Fox_int, Fox_rid *, Fox_rect2 *);
	Fox_int (*get_external_texture_for_eye)(void *, Fox_int);
	Fox_int (*get_external_depth_for_eye)(void *, Fox_int);
} Fox_xr_interface_gdnative;

void GDAPI Fox_xr_register_interface(const Fox_xr_interface_gdnative *p_interface);

// helper functions to access XRServer data
Fox_real_t GDAPI Fox_xr_get_worldscale();
Fox_transform3d GDAPI Fox_xr_get_reference_frame();

// helper functions for rendering
void GDAPI Fox_xr_blit(Fox_int p_eye, Fox_rid *p_render_target, Fox_rect2 *p_rect);
Fox_int GDAPI Fox_xr_get_texid(Fox_rid *p_render_target);

// helper functions for updating XR controllers
Fox_int GDAPI Fox_xr_add_controller(char *p_device_name, Fox_int p_hand, Fox_bool p_tracks_orientation, Fox_bool p_tracks_position);
void GDAPI Fox_xr_remove_controller(Fox_int p_controller_id);
void GDAPI Fox_xr_set_controller_transform(Fox_int p_controller_id, Fox_transform3d *p_transform, Fox_bool p_tracks_orientation, Fox_bool p_tracks_position);
void GDAPI Fox_xr_set_controller_button(Fox_int p_controller_id, Fox_int p_button, Fox_bool p_is_pressed);
void GDAPI Fox_xr_set_controller_axis(Fox_int p_controller_id, Fox_int p_axis, Fox_real_t p_value, Fox_bool p_can_be_negative);
Fox_real_t GDAPI Fox_xr_get_controller_rumble(Fox_int p_controller_id);

#ifdef __cplusplus
}
#endif

#endif /* !Fox_NATIVEXR_H */
