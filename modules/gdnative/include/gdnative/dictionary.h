/*************************************************************************/
/*  dictionary.h                                                         */
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

#ifndef Fox_DICTIONARY_H
#define Fox_DICTIONARY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define Fox_DICTIONARY_SIZE sizeof(void *)

#ifndef Fox_CORE_API_Fox_DICTIONARY_TYPE_DEFINED
#define Fox_CORE_API_Fox_DICTIONARY_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[Fox_DICTIONARY_SIZE];
} Fox_dictionary;
#endif

#include <gdnative/gdnative.h>
#include <gdnative/variant_struct.h>

void GDAPI Fox_dictionary_new(Fox_dictionary *p_self);
void GDAPI Fox_dictionary_new_copy(Fox_dictionary *r_dest, const Fox_dictionary *p_src);
void GDAPI Fox_dictionary_destroy(Fox_dictionary *p_self);
Fox_variant GDAPI *Fox_dictionary_operator_index(Fox_dictionary *p_self, const Fox_variant *p_key);
const Fox_variant GDAPI *Fox_dictionary_operator_index_const(const Fox_dictionary *p_self, const Fox_variant *p_key);

#ifdef __cplusplus
}
#endif

#endif // Fox_DICTIONARY_H
