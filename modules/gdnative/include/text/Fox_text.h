/*************************************************************************/
/*  Fox_text.h                                                         */
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

#ifndef Fox_NATIVETEXT_H
#define Fox_NATIVETEXT_H

#include <gdnative/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

#define Fox_TEXT_API_MAJOR 1
#define Fox_TEXT_API_MINOR 0

#define Fox_GLYPH_SIZE 40

#ifndef Fox_TEXT_API_Fox_GLYPH_TYPE_DEFINED
#define Fox_TEXT_API_Fox_GLYPH_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[Fox_GLYPH_SIZE];
} Fox_glyph;
#endif

#define Fox_PACKED_GLYPH_ARRAY_SIZE (2 * sizeof(void *))

#ifndef Fox_TEXT_API_Fox_PACKED_GLYPH_ARRAY_TYPE_DEFINED
#define Fox_TEXT_API_Fox_PACKED_GLYPH_ARRAY_TYPE_DEFINED
typedef struct {
	uint8_t _dont_touch_that[Fox_PACKED_GLYPH_ARRAY_SIZE];
} Fox_packed_glyph_array;
#endif

typedef struct {
	Fox_gdnative_api_version version;
	void *(*constructor)(Fox_object *);
	void (*destructor)(void *);
	Fox_string (*get_name)(const void *);
	Fox_bool (*has_feature)(const void *, Fox_int);
	bool (*load_support_data)(void *, const Fox_string *);
	Fox_string (*get_support_data_filename)(const void *);
	Fox_string (*get_support_data_info)(const void *);
	bool (*save_support_data)(void *, const Fox_string *);
	bool (*is_locale_right_to_left)(void *, const Fox_string *);
	void (*free)(void *, Fox_rid *);
	bool (*has)(void *, Fox_rid *);
	Fox_rid (*create_font_system)(void *, const Fox_string *, int);
	Fox_rid (*create_font_resource)(void *, const Fox_string *, int);
	Fox_rid (*create_font_memory)(void *, const uint8_t *, size_t, Fox_string *, int);
	Fox_rid (*create_font_bitmap)(void *, float, float, int);
	void (*font_bitmap_add_texture)(void *, Fox_rid *, const Fox_object *);
	void (*font_bitmap_add_char)(void *, Fox_rid *, char32_t, int, const Fox_rect2 *, const Fox_vector2 *, float);
	void (*font_bitmap_add_kerning_pair)(void *, Fox_rid *, char32_t, char32_t, int);
	float (*font_get_height)(void *, Fox_rid *, int);
	float (*font_get_ascent)(void *, Fox_rid *, int);
	float (*font_get_descent)(void *, Fox_rid *, int);
	float (*font_get_underline_position)(void *, Fox_rid *, int);
	float (*font_get_underline_thickness)(void *, Fox_rid *, int);
	int (*font_get_spacing_space)(void *, Fox_rid *);
	void (*font_set_spacing_space)(void *, Fox_rid *, int);
	int (*font_get_spacing_glyph)(void *, Fox_rid *);
	void (*font_set_spacing_glyph)(void *, Fox_rid *, int);
	void (*font_set_antialiased)(void *, Fox_rid *, bool);
	bool (*font_get_antialiased)(void *, Fox_rid *);
	Fox_dictionary (*font_get_feature_list)(void *, Fox_rid *);
	Fox_dictionary (*font_get_variation_list)(void *, Fox_rid *);
	void (*font_set_variation)(void *, Fox_rid *, const Fox_string *, double);
	double (*font_get_variation)(void *, Fox_rid *, const Fox_string *);
	void (*font_set_distance_field_hint)(void *, Fox_rid *, bool);
	bool (*font_get_distance_field_hint)(void *, Fox_rid *);
	void (*font_set_hinting)(void *, Fox_rid *, Fox_int);
	Fox_int (*font_get_hinting)(void *, Fox_rid *);
	void (*font_set_force_autohinter)(void *, Fox_rid *, bool);
	bool (*font_get_force_autohinter)(void *, Fox_rid *);
	bool (*font_has_char)(void *, Fox_rid *, char32_t);
	Fox_string (*font_get_supported_chars)(void *, Fox_rid *);
	bool (*font_has_outline)(void *, Fox_rid *);
	int (*font_get_base_size)(void *, Fox_rid *);
	bool (*font_is_language_supported)(void *, Fox_rid *, const Fox_string *);
	void (*font_set_language_support_override)(void *, Fox_rid *, const Fox_string *, bool);
	bool (*font_get_language_support_override)(void *, Fox_rid *, const Fox_string *);
	void (*font_remove_language_support_override)(void *, Fox_rid *, const Fox_string *);
	Fox_packed_string_array (*font_get_language_support_overrides)(void *, Fox_rid *);
	bool (*font_is_script_supported)(void *, Fox_rid *, const Fox_string *);
	void (*font_set_script_support_override)(void *, Fox_rid *, const Fox_string *, bool);
	bool (*font_get_script_support_override)(void *, Fox_rid *, const Fox_string *);
	void (*font_remove_script_support_override)(void *, Fox_rid *, const Fox_string *);
	Fox_packed_string_array (*font_get_script_support_overrides)(void *, Fox_rid *);
	uint32_t (*font_get_glyph_index)(void *, Fox_rid *, char32_t, char32_t);
	Fox_vector2 (*font_get_glyph_advance)(void *, Fox_rid *, uint32_t, int);
	Fox_vector2 (*font_get_glyph_kerning)(void *, Fox_rid *, uint32_t, uint32_t, int);
	Fox_vector2 (*font_draw_glyph)(void *, Fox_rid *, Fox_rid *, int, const Fox_vector2 *, uint32_t, const Fox_color *);
	Fox_vector2 (*font_draw_glyph_outline)(void *, Fox_rid *, Fox_rid *, int, int, const Fox_vector2 *, uint32_t, const Fox_color *);
	bool (*font_get_glyph_contours)(void *, Fox_rid *, int, uint32_t, Fox_packed_vector3_array *, Fox_packed_int32_array *, bool *);
	float (*font_get_oversampling)(void *);
	void (*font_set_oversampling)(void *, float);
	Fox_packed_string_array (*get_system_fonts)(void *);
	Fox_rid (*create_shaped_text)(void *, Fox_int, Fox_int);
	void (*shaped_text_clear)(void *, Fox_rid *);
	void (*shaped_text_set_direction)(void *, Fox_rid *, Fox_int);
	Fox_int (*shaped_text_get_direction)(void *, Fox_rid *);
	void (*shaped_text_set_bidi_override)(void *, Fox_rid *, const Fox_packed_vector2i_array *);
	void (*shaped_text_set_orientation)(void *, Fox_rid *, Fox_int);
	Fox_int (*shaped_text_get_orientation)(void *, Fox_rid *);
	void (*shaped_text_set_preserve_invalid)(void *, Fox_rid *, bool);
	bool (*shaped_text_get_preserve_invalid)(void *, Fox_rid *);
	void (*shaped_text_set_preserve_control)(void *, Fox_rid *, bool);
	bool (*shaped_text_get_preserve_control)(void *, Fox_rid *);
	bool (*shaped_text_add_string)(void *, Fox_rid *, const Fox_string *, const Fox_rid **, int, const Fox_dictionary *, const Fox_string *);
	bool (*shaped_text_add_object)(void *, Fox_rid *, const Fox_variant *, const Fox_vector2 *, Fox_int, Fox_int);
	bool (*shaped_text_resize_object)(void *, Fox_rid *, const Fox_variant *, const Fox_vector2 *, Fox_int);
	Fox_rid (*shaped_text_substr)(void *, Fox_rid *, Fox_int, Fox_int);
	Fox_rid (*shaped_text_get_parent)(void *, Fox_rid *);
	float (*shaped_text_fit_to_width)(void *, Fox_rid *, float, uint8_t);
	float (*shaped_text_tab_align)(void *, Fox_rid *, Fox_packed_float32_array *);
	bool (*shaped_text_shape)(void *, Fox_rid *);
	bool (*shaped_text_update_breaks)(void *, Fox_rid *);
	bool (*shaped_text_update_justification_ops)(void *, Fox_rid *);
	void (*shaped_text_overrun_trim_to_width)(void *, Fox_rid *, float, uint8_t);
	bool (*shaped_text_is_ready)(void *, Fox_rid *);
	Fox_packed_glyph_array (*shaped_text_get_glyphs)(void *, Fox_rid *);
	Fox_vector2i (*shaped_text_get_range)(void *, Fox_rid *);
	Fox_packed_glyph_array (*shaped_text_sort_logical)(void *, Fox_rid *);
	Fox_packed_vector2i_array (*shaped_text_get_line_breaks_adv)(void *, Fox_rid *, Fox_packed_float32_array *, int, bool, uint8_t);
	Fox_packed_vector2i_array (*shaped_text_get_line_breaks)(void *, Fox_rid *, float, int, uint8_t);
	Fox_packed_vector2i_array (*shaped_text_get_word_breaks)(void *, Fox_rid *, int);
	Fox_array (*shaped_text_get_objects)(void *, Fox_rid *);
	Fox_rect2 (*shaped_text_get_object_rect)(void *, Fox_rid *, const Fox_variant *);
	Fox_vector2 (*shaped_text_get_size)(void *, Fox_rid *);
	float (*shaped_text_get_ascent)(void *, Fox_rid *);
	float (*shaped_text_get_descent)(void *, Fox_rid *);
	float (*shaped_text_get_width)(void *, Fox_rid *);
	float (*shaped_text_get_underline_position)(void *, Fox_rid *);
	float (*shaped_text_get_underline_thickness)(void *, Fox_rid *);
	Fox_string (*format_number)(void *, const Fox_string *, const Fox_string *);
	Fox_string (*parse_number)(void *, const Fox_string *, const Fox_string *);
	Fox_string (*percent_sign)(void *, const Fox_string *);
} Fox_text_interface_gdnative;

void GDAPI Fox_text_register_interface(const Fox_text_interface_gdnative *p_interface, const Fox_string *p_name, uint32_t p_features);

// Glyph

void GDAPI Fox_glyph_new(Fox_glyph *r_dest);

Fox_vector2i GDAPI Fox_glyph_get_range(const Fox_glyph *p_self);
void GDAPI Fox_glyph_set_range(Fox_glyph *p_self, const Fox_vector2i *p_range);

Fox_int GDAPI Fox_glyph_get_count(const Fox_glyph *p_self);
void GDAPI Fox_glyph_set_count(Fox_glyph *p_self, Fox_int p_count);

Fox_int GDAPI Fox_glyph_get_repeat(const Fox_glyph *p_self);
void GDAPI Fox_glyph_set_repeat(Fox_glyph *p_self, Fox_int p_repeat);

Fox_int GDAPI Fox_glyph_get_flags(const Fox_glyph *p_self);
void GDAPI Fox_glyph_set_flags(Fox_glyph *p_self, Fox_int p_flags);

Fox_vector2 GDAPI Fox_glyph_get_offset(const Fox_glyph *p_self);
void GDAPI Fox_glyph_set_offset(Fox_glyph *p_self, const Fox_vector2 *p_offset);

Fox_float GDAPI Fox_glyph_get_advance(const Fox_glyph *p_self);
void GDAPI Fox_glyph_set_advance(Fox_glyph *p_self, Fox_float p_advance);

Fox_rid GDAPI Fox_glyph_get_font(const Fox_glyph *p_self);
void GDAPI Fox_glyph_set_font(Fox_glyph *p_self, Fox_rid *p_font);

Fox_int GDAPI Fox_glyph_get_font_size(const Fox_glyph *p_self);
void GDAPI Fox_glyph_set_font_size(Fox_glyph *p_self, Fox_int p_size);

Fox_int GDAPI Fox_glyph_get_index(const Fox_glyph *p_self);
void GDAPI Fox_glyph_set_index(Fox_glyph *p_self, Fox_int p_index);

// GlyphArray

void GDAPI Fox_packed_glyph_array_new(Fox_packed_glyph_array *r_dest);
void GDAPI Fox_packed_glyph_array_new_copy(Fox_packed_glyph_array *r_dest, const Fox_packed_glyph_array *p_src);

const Fox_glyph GDAPI *Fox_packed_glyph_array_ptr(const Fox_packed_glyph_array *p_self);
Fox_glyph GDAPI *Fox_packed_glyph_array_ptrw(Fox_packed_glyph_array *p_self);

void GDAPI Fox_packed_glyph_array_append(Fox_packed_glyph_array *p_self, const Fox_glyph *p_data);

void GDAPI Fox_packed_glyph_array_append_array(Fox_packed_glyph_array *p_self, const Fox_packed_glyph_array *p_array);

Fox_error GDAPI Fox_packed_glyph_array_insert(Fox_packed_glyph_array *p_self, const Fox_int p_idx, const Fox_glyph *p_data);

Fox_bool GDAPI Fox_packed_glyph_array_has(Fox_packed_glyph_array *p_self, const Fox_glyph *p_value);

void GDAPI Fox_packed_glyph_array_sort(Fox_packed_glyph_array *p_self);

void GDAPI Fox_packed_glyph_array_reverse(Fox_packed_glyph_array *p_self);

void GDAPI Fox_packed_glyph_array_push_back(Fox_packed_glyph_array *p_self, const Fox_glyph *p_data);

void GDAPI Fox_packed_glyph_array_remove(Fox_packed_glyph_array *p_self, Fox_int p_idx);

void GDAPI Fox_packed_glyph_array_resize(Fox_packed_glyph_array *p_self, Fox_int p_size);

void GDAPI Fox_packed_glyph_array_set(Fox_packed_glyph_array *p_self, Fox_int p_idx, const Fox_glyph *p_data);
Fox_glyph GDAPI Fox_packed_glyph_array_get(const Fox_packed_glyph_array *p_self, Fox_int p_idx);

Fox_int GDAPI Fox_packed_glyph_array_size(const Fox_packed_glyph_array *p_self);

Fox_bool GDAPI Fox_packed_glyph_array_is_empty(const Fox_packed_glyph_array *p_self);

void GDAPI Fox_packed_glyph_array_destroy(Fox_packed_glyph_array *p_self);

// Grapheme

#ifdef __cplusplus
}
#endif

#endif /* !Fox_NATIVETEXT_H */
