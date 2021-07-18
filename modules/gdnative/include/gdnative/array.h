/*************************************************************************/
/*  array.h                                                              */
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

#ifndef Fox_ARRAY_H
#define Fox_ARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define Fox_ARRAY_SIZE sizeof(void *)

#ifndef Fox_CORE_API_Fox_ARRAY_TYPE_DEFINED
#define Fox_CORE_API_Fox_ARRAY_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[Fox_ARRAY_SIZE];
} Fox_array;
#endif

#include <gdnative/gdnative.h>
#include <gdnative/variant_struct.h>

void GDAPI Fox_array_new(Fox_array *p_self);
void GDAPI Fox_array_new_copy(Fox_array *r_dest, const Fox_array *p_src);
void GDAPI Fox_array_destroy(Fox_array *p_self);
Fox_variant GDAPI *Fox_array_operator_index(Fox_array *p_self, Fox_int p_index);
const Fox_variant GDAPI *Fox_array_operator_index_const(const Fox_array *p_self, Fox_int p_index);

#ifdef __cplusplus
}
#endif

#endif // Fox_ARRAY_H
