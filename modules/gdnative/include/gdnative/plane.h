/*************************************************************************/
/*  plane.h                                                              */
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

#ifndef Fox_PLANE_H
#define Fox_PLANE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <gdnative/math_defs.h>

#define Fox_PLANE_SIZE (sizeof(Fox_real_t) * 4)

#ifndef Fox_CORE_API_Fox_PLANE_TYPE_DEFINED
#define Fox_CORE_API_Fox_PLANE_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[Fox_PLANE_SIZE];
} Fox_plane;
#endif

#include <gdnative/gdnative.h>

void GDAPI Fox_plane_new(Fox_plane *p_self);
void GDAPI Fox_plane_new_copy(Fox_plane *r_dest, const Fox_plane *p_src);

#ifdef __cplusplus
}
#endif

#endif // Fox_PLANE_H
