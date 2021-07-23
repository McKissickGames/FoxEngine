/*************************************************************************/
/*  text_server_gdnative.cpp                                             */
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

#include "text_server_gdnative.h"

bool TextServerGDNative::has_feature(Feature p_feature) {
	ERR_FAIL_COND_V(interface == nullptr, false);
	return interface->has_feature(data, (Fox_int)p_feature);
}

String TextServerGDNative::get_name() const {
	ERR_FAIL_COND_V(interface == nullptr, String());
	Fox_string result = interface->get_name(data);
	String name = *(String *)&result;
	Fox_string_destroy(&result);
	return name;
}

void TextServerGDNative::free(RID p_rid) {
	ERR_FAIL_COND(interface == nullptr);
	interface->free(data, (Fox_rid *)&p_rid);
}

bool TextServerGDNative::has(RID p_rid) {
	ERR_FAIL_COND_V(interface == nullptr, false);
	return interface->has(data, (Fox_rid *)&p_rid);
}

bool TextServerGDNative::load_support_data(const String &p_filename) {
	ERR_FAIL_COND_V(interface == nullptr, false);
	return interface->load_support_data(data, (Fox_string *)&p_filename);
}

#ifdef TOOLS_ENABLED

String TextServerGDNative::get_support_data_filename() {
	ERR_FAIL_COND_V(interface == nullptr, String());
	Fox_string result = interface->get_support_data_filename(data);
	String name = *(String *)&result;
	Fox_string_destroy(&result);
	return name;
}

String TextServerGDNative::get_support_data_info() {
	ERR_FAIL_COND_V(interface == nullptr, String());
	Fox_string result = interface->get_support_data_info(data);
	String info = *(String *)&result;
	Fox_string_destroy(&result);
	return info;
}

bool TextServerGDNative::save_support_data(const String &p_filename) {
	ERR_FAIL_COND_V(interface == nullptr, false);
	return interface->save_support_data(data, (Fox_string *)&p_filename);
}

#endif

bool TextServerGDNative::is_locale_right_to_left(const String &p_locale) {
	ERR_FAIL_COND_V(interface == nullptr, false);
	return interface->is_locale_right_to_left(data, (Fox_string *)&p_locale);
}

/*************************************************************************/
/* Font interface */
/*************************************************************************/

RID TextServerGDNative::create_font_system(const String &p_name, int p_base_size) {
	ERR_FAIL_COND_V(interface == nullptr, RID());
	Fox_rid result = interface->create_font_system(data, (const Fox_string *)&p_name, p_base_size);
	RID rid = *(RID *)&result;
	return rid;
}

RID TextServerGDNative::create_font_resource(const String &p_filename, int p_base_size) {
	ERR_FAIL_COND_V(interface == nullptr, RID());
	Fox_rid result = interface->create_font_resource(data, (const Fox_string *)&p_filename, p_base_size);
	RID rid = *(RID *)&result;
	return rid;
}

RID TextServerGDNative::create_font_memory(const uint8_t *p_data, size_t p_size, const String &p_type, int p_base_size) {
	ERR_FAIL_COND_V(interface == nullptr, RID());
	Fox_rid result = interface->create_font_memory(data, p_data, p_size, (Fox_string *)&p_type, p_base_size);
	RID rid = *(RID *)&result;
	return rid;
}

RID TextServerGDNative::create_font_bitmap(float p_height, float p_ascent, int p_base_size) {
	ERR_FAIL_COND_V(interface == nullptr, RID());
	Fox_rid result = interface->create_font_bitmap(data, p_height, p_ascent, p_base_size);
	RID rid = *(RID *)&result;
	return rid;
}

void TextServerGDNative::font_bitmap_add_texture(RID p_font, const Ref<Texture> &p_texture) {
	ERR_FAIL_COND(interface == nullptr);
	interface->font_bitmap_add_texture(data, (Fox_rid *)&p_font, (const Fox_object *)p_texture.ptr());
}

void TextServerGDNative::font_bitmap_add_char(RID p_font, char32_t p_char, int p_texture_idx, const Rect2 &p_rect, const Size2 &p_align, float p_advance) {
	ERR_FAIL_COND(interface == nullptr);
	interface->font_bitmap_add_char(data, (Fox_rid *)&p_font, p_char, p_texture_idx, (const Fox_rect2 *)&p_rect, (const Fox_vector2 *)&p_align, p_advance);
}

void TextServerGDNative::font_bitmap_add_kerning_pair(RID p_font, char32_t p_A, char32_t p_B, int p_kerning) {
	ERR_FAIL_COND(interface == nullptr);
	interface->font_bitmap_add_kerning_pair(data, (Fox_rid *)&p_font, p_A, p_B, p_kerning);
}

float TextServerGDNative::font_get_height(RID p_font, int p_size) const {
	ERR_FAIL_COND_V(interface == nullptr, 0.f);
	return interface->font_get_height(data, (Fox_rid *)&p_font, p_size);
}

float TextServerGDNative::font_get_ascent(RID p_font, int p_size) const {
	ERR_FAIL_COND_V(interface == nullptr, 0.f);
	return interface->font_get_ascent(data, (Fox_rid *)&p_font, p_size);
}

float TextServerGDNative::font_get_descent(RID p_font, int p_size) const {
	ERR_FAIL_COND_V(interface == nullptr, 0.f);
	return interface->font_get_descent(data, (Fox_rid *)&p_font, p_size);
}

float TextServerGDNative::font_get_underline_position(RID p_font, int p_size) const {
	ERR_FAIL_COND_V(interface == nullptr, 0.f);
	return interface->font_get_underline_position(data, (Fox_rid *)&p_font, p_size);
}

float TextServerGDNative::font_get_underline_thickness(RID p_font, int p_size) const {
	ERR_FAIL_COND_V(interface == nullptr, 0.f);
	return interface->font_get_underline_thickness(data, (Fox_rid *)&p_font, p_size);
}

int TextServerGDNative::font_get_spacing_space(RID p_font) const {
	ERR_FAIL_COND_V(interface == nullptr, 0);
	return interface->font_get_spacing_space(data, (Fox_rid *)&p_font);
}

void TextServerGDNative::font_set_spacing_space(RID p_font, int p_value) {
	ERR_FAIL_COND(interface == nullptr);
	interface->font_set_spacing_space(data, (Fox_rid *)&p_font, p_value);
}

int TextServerGDNative::font_get_spacing_glyph(RID p_font) const {
	ERR_FAIL_COND_V(interface == nullptr, 0);
	return interface->font_get_spacing_glyph(data, (Fox_rid *)&p_font);
}

void TextServerGDNative::font_set_spacing_glyph(RID p_font, int p_value) {
	ERR_FAIL_COND(interface == nullptr);
	interface->font_set_spacing_glyph(data, (Fox_rid *)&p_font, p_value);
}

void TextServerGDNative::font_set_antialiased(RID p_font, bool p_antialiased) {
	ERR_FAIL_COND(interface == nullptr);
	interface->font_set_antialiased(data, (Fox_rid *)&p_font, p_antialiased);
}

bool TextServerGDNative::font_get_antialiased(RID p_font) const {
	ERR_FAIL_COND_V(interface == nullptr, false);
	return interface->font_get_antialiased(data, (Fox_rid *)&p_font);
}

Dictionary TextServerGDNative::font_get_variation_list(RID p_font) const {
	ERR_FAIL_COND_V(interface == nullptr, Dictionary());
	Fox_dictionary result = interface->font_get_variation_list(data, (Fox_rid *)&p_font);
	Dictionary info = *(Dictionary *)&result;
	Fox_dictionary_destroy(&result);

	return info;
}

void TextServerGDNative::font_set_variation(RID p_font, const String &p_name, double p_value) {
	ERR_FAIL_COND(interface == nullptr);
	interface->font_set_variation(data, (Fox_rid *)&p_font, (Fox_string *)&p_name, p_value);
}

double TextServerGDNative::font_get_variation(RID p_font, const String &p_name) const {
	return interface->font_get_variation(data, (Fox_rid *)&p_font, (Fox_string *)&p_name);
}

void TextServerGDNative::font_set_hinting(RID p_font, TextServer::Hinting p_hinting) {
	ERR_FAIL_COND(interface == nullptr);
	interface->font_set_hinting(data, (Fox_rid *)&p_font, (Fox_int)p_hinting);
}

TextServer::Hinting TextServerGDNative::font_get_hinting(RID p_font) const {
	ERR_FAIL_COND_V(interface == nullptr, TextServer::HINTING_NONE);
	return (TextServer::Hinting)interface->font_get_hinting(data, (Fox_rid *)&p_font);
}

Dictionary TextServerGDNative::font_get_feature_list(RID p_font) const {
	ERR_FAIL_COND_V(interface == nullptr, Dictionary());
	Fox_dictionary result = interface->font_get_feature_list(data, (Fox_rid *)&p_font);
	Dictionary info = *(Dictionary *)&result;
	Fox_dictionary_destroy(&result);

	return info;
}

void TextServerGDNative::font_set_distance_field_hint(RID p_font, bool p_distance_field) {
	ERR_FAIL_COND(interface == nullptr);
	interface->font_set_distance_field_hint(data, (Fox_rid *)&p_font, p_distance_field);
}

bool TextServerGDNative::font_get_distance_field_hint(RID p_font) const {
	ERR_FAIL_COND_V(interface == nullptr, false);
	return interface->font_get_distance_field_hint(data, (Fox_rid *)&p_font);
}

void TextServerGDNative::font_set_force_autohinter(RID p_font, bool p_enabeld) {
	ERR_FAIL_COND(interface == nullptr);
	interface->font_set_force_autohinter(data, (Fox_rid *)&p_font, p_enabeld);
}

bool TextServerGDNative::font_get_force_autohinter(RID p_font) const {
	ERR_FAIL_COND_V(interface == nullptr, false);
	return interface->font_get_force_autohinter(data, (Fox_rid *)&p_font);
}

bool TextServerGDNative::font_has_char(RID p_font, char32_t p_char) const {
	ERR_FAIL_COND_V(interface == nullptr, false);
	return interface->font_has_char(data, (Fox_rid *)&p_font, p_char);
}

String TextServerGDNative::font_get_supported_chars(RID p_font) const {
	ERR_FAIL_COND_V(interface == nullptr, String());
	Fox_string result = interface->font_get_supported_chars(data, (Fox_rid *)&p_font);
	String ret = *(String *)&result;
	Fox_string_destroy(&result);
	return ret;
}

bool TextServerGDNative::font_has_outline(RID p_font) const {
	ERR_FAIL_COND_V(interface == nullptr, false);
	return interface->font_has_outline(data, (Fox_rid *)&p_font);
}

float TextServerGDNative::font_get_base_size(RID p_font) const {
	ERR_FAIL_COND_V(interface == nullptr, 0.f);
	return interface->font_get_base_size(data, (Fox_rid *)&p_font);
}

bool TextServerGDNative::font_is_language_supported(RID p_font, const String &p_language) const {
	ERR_FAIL_COND_V(interface == nullptr, false);
	return interface->font_is_language_supported(data, (Fox_rid *)&p_font, (Fox_string *)&p_language);
}

void TextServerGDNative::font_set_language_support_override(RID p_font, const String &p_language, bool p_supported) {
	ERR_FAIL_COND(interface == nullptr);
	return interface->font_set_language_support_override(data, (Fox_rid *)&p_font, (Fox_string *)&p_language, p_supported);
}

bool TextServerGDNative::font_get_language_support_override(RID p_font, const String &p_language) {
	ERR_FAIL_COND_V(interface == nullptr, false);
	return interface->font_get_language_support_override(data, (Fox_rid *)&p_font, (Fox_string *)&p_language);
}

void TextServerGDNative::font_remove_language_support_override(RID p_font, const String &p_language) {
	ERR_FAIL_COND(interface == nullptr);
	interface->font_remove_language_support_override(data, (Fox_rid *)&p_font, (Fox_string *)&p_language);
}

Vector<String> TextServerGDNative::font_get_language_support_overrides(RID p_font) {
	ERR_FAIL_COND_V(interface == nullptr, Vector<String>());
	Fox_packed_string_array result = interface->font_get_language_support_overrides(data, (Fox_rid *)&p_font);
	Vector<String> ret = *(Vector<String> *)&result;
	Fox_packed_string_array_destroy(&result);
	return ret;
}

bool TextServerGDNative::font_is_script_supported(RID p_font, const String &p_script) const {
	ERR_FAIL_COND_V(interface == nullptr, false);
	return interface->font_is_script_supported(data, (Fox_rid *)&p_font, (Fox_string *)&p_script);
}

void TextServerGDNative::font_set_script_support_override(RID p_font, const String &p_script, bool p_supported) {
	ERR_FAIL_COND(interface == nullptr);
	return interface->font_set_script_support_override(data, (Fox_rid *)&p_font, (Fox_string *)&p_script, p_supported);
}

bool TextServerGDNative::font_get_script_support_override(RID p_font, const String &p_script) {
	ERR_FAIL_COND_V(interface == nullptr, false);
	return interface->font_get_script_support_override(data, (Fox_rid *)&p_font, (Fox_string *)&p_script);
}

void TextServerGDNative::font_remove_script_support_override(RID p_font, const String &p_script) {
	ERR_FAIL_COND(interface == nullptr);
	interface->font_remove_script_support_override(data, (Fox_rid *)&p_font, (Fox_string *)&p_script);
}

Vector<String> TextServerGDNative::font_get_script_support_overrides(RID p_font) {
	ERR_FAIL_COND_V(interface == nullptr, Vector<String>());
	Fox_packed_string_array result = interface->font_get_script_support_overrides(data, (Fox_rid *)&p_font);
	Vector<String> ret = *(Vector<String> *)&result;
	Fox_packed_string_array_destroy(&result);
	return ret;
}

uint32_t TextServerGDNative::font_get_glyph_index(RID p_font, char32_t p_char, char32_t p_variation_selector) const {
	ERR_FAIL_COND_V(interface == nullptr, 0);
	return interface->font_get_glyph_index(data, (Fox_rid *)&p_font, p_char, p_variation_selector);
}

Vector2 TextServerGDNative::font_get_glyph_advance(RID p_font, uint32_t p_index, int p_size) const {
	ERR_FAIL_COND_V(interface == nullptr, Vector2());
	Fox_vector2 result = interface->font_get_glyph_advance(data, (Fox_rid *)&p_font, p_index, p_size);
	Vector2 advance = *(Vector2 *)&result;
	return advance;
}

Vector2 TextServerGDNative::font_get_glyph_kerning(RID p_font, uint32_t p_index_a, uint32_t p_index_b, int p_size) const {
	ERR_FAIL_COND_V(interface == nullptr, Vector2());
	Fox_vector2 result = interface->font_get_glyph_kerning(data, (Fox_rid *)&p_font, p_index_a, p_index_b, p_size);
	Vector2 kerning = *(Vector2 *)&result;
	return kerning;
}

Vector2 TextServerGDNative::font_draw_glyph(RID p_font, RID p_canvas, int p_size, const Vector2 &p_pos, uint32_t p_index, const Color &p_color) const {
	ERR_FAIL_COND_V(interface == nullptr, Vector2());
	Fox_vector2 result = interface->font_draw_glyph(data, (Fox_rid *)&p_font, (Fox_rid *)&p_canvas, p_size, (const Fox_vector2 *)&p_pos, p_index, (const Fox_color *)&p_color);
	Vector2 advance = *(Vector2 *)&result;
	return advance;
}

Vector2 TextServerGDNative::font_draw_glyph_outline(RID p_font, RID p_canvas, int p_size, int p_outline_size, const Vector2 &p_pos, uint32_t p_index, const Color &p_color) const {
	ERR_FAIL_COND_V(interface == nullptr, Vector2());
	Fox_vector2 result = interface->font_draw_glyph_outline(data, (Fox_rid *)&p_font, (Fox_rid *)&p_canvas, p_size, p_outline_size, (const Fox_vector2 *)&p_pos, p_index, (const Fox_color *)&p_color);
	Vector2 advance = *(Vector2 *)&result;
	return advance;
}

bool TextServerGDNative::font_get_glyph_contours(RID p_font, int p_size, uint32_t p_index, Vector<Vector3> &r_points, Vector<int32_t> &r_contours, bool &r_orientation) const {
	ERR_FAIL_COND_V(interface == nullptr, false);
	ERR_FAIL_COND_V(interface->font_get_glyph_contours == nullptr, false);
	return interface->font_get_glyph_contours(data, (Fox_rid *)&p_font, p_size, p_index, (Fox_packed_vector3_array *)&r_points, (Fox_packed_int32_array *)&r_contours, (bool *)&r_orientation);
}

float TextServerGDNative::font_get_oversampling() const {
	ERR_FAIL_COND_V(interface == nullptr, 1.f);
	return interface->font_get_oversampling(data);
}

void TextServerGDNative::font_set_oversampling(float p_oversampling) {
	ERR_FAIL_COND(interface == nullptr);
	return interface->font_set_oversampling(data, p_oversampling);
}

Vector<String> TextServerGDNative::get_system_fonts() const {
	ERR_FAIL_COND_V(interface == nullptr, Vector<String>());
	Fox_packed_string_array result = interface->get_system_fonts(data);
	Vector<String> fonts = *(Vector<String> *)&result;
	Fox_packed_string_array_destroy(&result);
	return fonts;
}

/*************************************************************************/
/* Shaped text buffer interface                                          */
/*************************************************************************/

RID TextServerGDNative::create_shaped_text(TextServer::Direction p_direction, TextServer::Orientation p_orientation) {
	ERR_FAIL_COND_V(interface == nullptr, RID());
	Fox_rid result = interface->create_shaped_text(data, (Fox_int)p_direction, (Fox_int)p_orientation);
	RID rid = *(RID *)&result;
	return rid;
}

void TextServerGDNative::shaped_text_clear(RID p_shaped) {
	ERR_FAIL_COND(interface == nullptr);
	interface->shaped_text_clear(data, (Fox_rid *)&p_shaped);
}

void TextServerGDNative::shaped_text_set_direction(RID p_shaped, TextServer::Direction p_direction) {
	ERR_FAIL_COND(interface == nullptr);
	interface->shaped_text_set_direction(data, (Fox_rid *)&p_shaped, (Fox_int)p_direction);
}

TextServer::Direction TextServerGDNative::shaped_text_get_direction(RID p_shaped) const {
	ERR_FAIL_COND_V(interface == nullptr, TextServer::DIRECTION_LTR);
	return (TextServer::Direction)interface->shaped_text_get_direction(data, (Fox_rid *)&p_shaped);
}

void TextServerGDNative::shaped_text_set_orientation(RID p_shaped, TextServer::Orientation p_orientation) {
	ERR_FAIL_COND(interface == nullptr);
	interface->shaped_text_set_orientation(data, (Fox_rid *)&p_shaped, (Fox_int)p_orientation);
}

TextServer::Orientation TextServerGDNative::shaped_text_get_orientation(RID p_shaped) const {
	ERR_FAIL_COND_V(interface == nullptr, TextServer::ORIENTATION_HORIZONTAL);
	return (TextServer::Orientation)interface->shaped_text_get_orientation(data, (Fox_rid *)&p_shaped);
}

void TextServerGDNative::shaped_text_set_bidi_override(RID p_shaped, const Vector<Vector2i> &p_override) {
	ERR_FAIL_COND(interface == nullptr);
	interface->shaped_text_set_bidi_override(data, (Fox_rid *)&p_shaped, (const Fox_packed_vector2i_array *)&p_override);
}

void TextServerGDNative::shaped_text_set_preserve_invalid(RID p_shaped, bool p_enabled) {
	ERR_FAIL_COND(interface == nullptr);
	interface->shaped_text_set_preserve_invalid(data, (Fox_rid *)&p_shaped, p_enabled);
}

bool TextServerGDNative::shaped_text_get_preserve_invalid(RID p_shaped) const {
	ERR_FAIL_COND_V(interface == nullptr, false);
	return (TextServer::Orientation)interface->shaped_text_get_preserve_invalid(data, (Fox_rid *)&p_shaped);
}

void TextServerGDNative::shaped_text_set_preserve_control(RID p_shaped, bool p_enabled) {
	ERR_FAIL_COND(interface == nullptr);
	interface->shaped_text_set_preserve_control(data, (Fox_rid *)&p_shaped, p_enabled);
}

bool TextServerGDNative::shaped_text_get_preserve_control(RID p_shaped) const {
	ERR_FAIL_COND_V(interface == nullptr, false);
	return (TextServer::Orientation)interface->shaped_text_get_preserve_control(data, (Fox_rid *)&p_shaped);
}

bool TextServerGDNative::shaped_text_add_string(RID p_shaped, const String &p_text, const Vector<RID> &p_fonts, int p_size, const Dictionary &p_opentype_features, const String &p_language) {
	ERR_FAIL_COND_V(interface == nullptr, false);
	return interface->shaped_text_add_string(data, (Fox_rid *)&p_shaped, (const Fox_string *)&p_text, (const Fox_rid **)p_fonts.ptr(), p_size, (const Fox_dictionary *)&p_opentype_features, (const Fox_string *)&p_language);
}

bool TextServerGDNative::shaped_text_add_object(RID p_shaped, Variant p_key, const Size2 &p_size, VAlign p_inline_align, int p_length) {
	ERR_FAIL_COND_V(interface == nullptr, false);
	return interface->shaped_text_add_object(data, (Fox_rid *)&p_shaped, (const Fox_variant *)&p_key, (const Fox_vector2 *)&p_size, (Fox_int)p_inline_align, p_length);
}

bool TextServerGDNative::shaped_text_resize_object(RID p_shaped, Variant p_key, const Size2 &p_size, VAlign p_inline_align) {
	ERR_FAIL_COND_V(interface == nullptr, false);
	return interface->shaped_text_resize_object(data, (Fox_rid *)&p_shaped, (const Fox_variant *)&p_key, (const Fox_vector2 *)&p_size, (Fox_int)p_inline_align);
}

RID TextServerGDNative::shaped_text_substr(RID p_shaped, int p_start, int p_length) const {
	ERR_FAIL_COND_V(interface == nullptr, RID());
	Fox_rid result = interface->shaped_text_substr(data, (Fox_rid *)&p_shaped, (Fox_int)p_start, (Fox_int)p_length);
	RID rid = *(RID *)&result;
	return rid;
}

RID TextServerGDNative::shaped_text_get_parent(RID p_shaped) const {
	ERR_FAIL_COND_V(interface == nullptr, RID());
	Fox_rid result = interface->shaped_text_get_parent(data, (Fox_rid *)&p_shaped);
	RID rid = *(RID *)&result;
	return rid;
}

float TextServerGDNative::shaped_text_fit_to_width(RID p_shaped, float p_width, uint8_t p_jst_flags) {
	ERR_FAIL_COND_V(interface == nullptr, 0.f);
	return interface->shaped_text_fit_to_width(data, (Fox_rid *)&p_shaped, p_width, p_jst_flags);
}

float TextServerGDNative::shaped_text_tab_align(RID p_shaped, const Vector<float> &p_tab_stops) {
	ERR_FAIL_COND_V(interface == nullptr, 0.f);
	return interface->shaped_text_tab_align(data, (Fox_rid *)&p_shaped, (Fox_packed_float32_array *)&p_tab_stops);
}

bool TextServerGDNative::shaped_text_shape(RID p_shaped) {
	ERR_FAIL_COND_V(interface == nullptr, false);
	return interface->shaped_text_shape(data, (Fox_rid *)&p_shaped);
}

bool TextServerGDNative::shaped_text_update_breaks(RID p_shaped) {
	ERR_FAIL_COND_V(interface == nullptr, false);
	return interface->shaped_text_update_breaks(data, (Fox_rid *)&p_shaped);
}

bool TextServerGDNative::shaped_text_update_justification_ops(RID p_shaped) {
	ERR_FAIL_COND_V(interface == nullptr, false);
	return interface->shaped_text_update_justification_ops(data, (Fox_rid *)&p_shaped);
}

void TextServerGDNative::shaped_text_overrun_trim_to_width(RID p_shaped_line, float p_width, uint8_t p_clip_flags) {
	ERR_FAIL_COND(interface == nullptr);
	interface->shaped_text_overrun_trim_to_width(data, (Fox_rid *)&p_shaped_line, p_width, p_clip_flags);
};

bool TextServerGDNative::shaped_text_is_ready(RID p_shaped) const {
	ERR_FAIL_COND_V(interface == nullptr, false);
	return interface->shaped_text_is_ready(data, (Fox_rid *)&p_shaped);
}

Vector<TextServer::Glyph> TextServerGDNative::shaped_text_get_glyphs(RID p_shaped) const {
	ERR_FAIL_COND_V(interface == nullptr, Vector<TextServer::Glyph>());
	Fox_packed_glyph_array result = interface->shaped_text_get_glyphs(data, (Fox_rid *)&p_shaped);
	Vector<TextServer::Glyph> glyphs = *(Vector<TextServer::Glyph> *)&result;
	Fox_packed_glyph_array_destroy(&result);
	return glyphs;
}

Vector2i TextServerGDNative::shaped_text_get_range(RID p_shaped) const {
	ERR_FAIL_COND_V(interface == nullptr, Vector2i());
	Fox_vector2i result = interface->shaped_text_get_range(data, (Fox_rid *)&p_shaped);
	Vector2i range = *(Vector2i *)&result;
	return range;
}

Vector<TextServer::Glyph> TextServerGDNative::shaped_text_sort_logical(RID p_shaped) {
	ERR_FAIL_COND_V(interface == nullptr, Vector<TextServer::Glyph>());
	Fox_packed_glyph_array result = interface->shaped_text_sort_logical(data, (Fox_rid *)&p_shaped);
	Vector<TextServer::Glyph> glyphs = *(Vector<TextServer::Glyph> *)&result;
	Fox_packed_glyph_array_destroy(&result);
	return glyphs;
}

Vector<Vector2i> TextServerGDNative::shaped_text_get_line_breaks_adv(RID p_shaped, const Vector<float> &p_width, int p_start, bool p_once, uint8_t p_break_flags) const {
	ERR_FAIL_COND_V(interface == nullptr, Vector<Vector2i>());
	if (interface->shaped_text_get_line_breaks_adv != nullptr) {
		Fox_packed_vector2i_array result = interface->shaped_text_get_line_breaks_adv(data, (Fox_rid *)&p_shaped, (Fox_packed_float32_array *)&p_width, p_start, p_once, p_break_flags);
		Vector<Vector2i> breaks = *(Vector<Vector2i> *)&result;
		Fox_packed_vector2i_array_destroy(&result);
		return breaks;
	} else {
		return TextServer::shaped_text_get_line_breaks_adv(p_shaped, p_width, p_break_flags);
	}
}

Vector<Vector2i> TextServerGDNative::shaped_text_get_line_breaks(RID p_shaped, float p_width, int p_start, uint8_t p_break_flags) const {
	ERR_FAIL_COND_V(interface == nullptr, Vector<Vector2i>());
	if (interface->shaped_text_get_line_breaks != nullptr) {
		Fox_packed_vector2i_array result = interface->shaped_text_get_line_breaks(data, (Fox_rid *)&p_shaped, p_width, p_start, p_break_flags);
		Vector<Vector2i> breaks = *(Vector<Vector2i> *)&result;
		Fox_packed_vector2i_array_destroy(&result);
		return breaks;
	} else {
		return TextServer::shaped_text_get_line_breaks(p_shaped, p_width, p_break_flags);
	}
}

Vector<Vector2i> TextServerGDNative::shaped_text_get_word_breaks(RID p_shaped, int p_grapheme_flags) const {
	ERR_FAIL_COND_V(interface == nullptr, Vector<Vector2i>());
	if (interface->shaped_text_get_word_breaks != nullptr) {
		Fox_packed_vector2i_array result = interface->shaped_text_get_word_breaks(data, (Fox_rid *)&p_shaped, p_grapheme_flags);
		Vector<Vector2i> breaks = *(Vector<Vector2i> *)&result;
		Fox_packed_vector2i_array_destroy(&result);
		return breaks;
	} else {
		return TextServer::shaped_text_get_word_breaks(p_shaped, p_grapheme_flags);
	}
}

Array TextServerGDNative::shaped_text_get_objects(RID p_shaped) const {
	ERR_FAIL_COND_V(interface == nullptr, Array());
	Fox_array result = interface->shaped_text_get_objects(data, (Fox_rid *)&p_shaped);
	Array rect = *(Array *)&result;
	return rect;
}

Rect2 TextServerGDNative::shaped_text_get_object_rect(RID p_shaped, Variant p_key) const {
	ERR_FAIL_COND_V(interface == nullptr, Rect2());
	Fox_rect2 result = interface->shaped_text_get_object_rect(data, (Fox_rid *)&p_shaped, (const Fox_variant *)&p_key);
	Rect2 rect = *(Rect2 *)&result;
	return rect;
}

Size2 TextServerGDNative::shaped_text_get_size(RID p_shaped) const {
	ERR_FAIL_COND_V(interface == nullptr, Size2());
	Fox_vector2 result = interface->shaped_text_get_size(data, (Fox_rid *)&p_shaped);
	Size2 size = *(Size2 *)&result;
	return size;
}

float TextServerGDNative::shaped_text_get_ascent(RID p_shaped) const {
	ERR_FAIL_COND_V(interface == nullptr, 0.f);
	return interface->shaped_text_get_ascent(data, (Fox_rid *)&p_shaped);
}

float TextServerGDNative::shaped_text_get_descent(RID p_shaped) const {
	ERR_FAIL_COND_V(interface == nullptr, 0.f);
	return interface->shaped_text_get_descent(data, (Fox_rid *)&p_shaped);
}

float TextServerGDNative::shaped_text_get_width(RID p_shaped) const {
	ERR_FAIL_COND_V(interface == nullptr, 0.f);
	return interface->shaped_text_get_width(data, (Fox_rid *)&p_shaped);
}

float TextServerGDNative::shaped_text_get_underline_position(RID p_shaped) const {
	ERR_FAIL_COND_V(interface == nullptr, 0.f);
	return interface->shaped_text_get_underline_position(data, (Fox_rid *)&p_shaped);
}

float TextServerGDNative::shaped_text_get_underline_thickness(RID p_shaped) const {
	ERR_FAIL_COND_V(interface == nullptr, 0.f);
	return interface->shaped_text_get_underline_thickness(data, (Fox_rid *)&p_shaped);
}

String TextServerGDNative::format_number(const String &p_string, const String &p_language) const {
	ERR_FAIL_COND_V(interface == nullptr, String());
	Fox_string result = interface->format_number(data, (const Fox_string *)&p_string, (const Fox_string *)&p_language);
	if (interface->format_number == nullptr) {
		return p_string;
	}
	String ret = *(String *)&result;
	Fox_string_destroy(&result);
	return ret;
}

String TextServerGDNative::parse_number(const String &p_string, const String &p_language) const {
	ERR_FAIL_COND_V(interface == nullptr, String());
	if (interface->parse_number == nullptr) {
		return p_string;
	}
	Fox_string result = interface->parse_number(data, (const Fox_string *)&p_string, (const Fox_string *)&p_language);
	String ret = *(String *)&result;
	Fox_string_destroy(&result);
	return ret;
}

String TextServerGDNative::percent_sign(const String &p_language) const {
	ERR_FAIL_COND_V(interface == nullptr, String());
	if (interface->percent_sign == nullptr) {
		return "%";
	}
	Fox_string result = interface->percent_sign(data, (const Fox_string *)&p_language);
	String ret = *(String *)&result;
	Fox_string_destroy(&result);
	return ret;
}

TextServer *TextServerGDNative::create_func(Error &r_error, void *p_user_data) {
	const Fox_text_interface_gdnative *interface = (const Fox_text_interface_gdnative *)p_user_data;
	r_error = OK;

	TextServerGDNative *server = memnew(TextServerGDNative());
	server->interface = interface;
	server->data = interface->constructor((Fox_object *)server);

	return server;
}

TextServerGDNative::TextServerGDNative() {
	data = nullptr;
	interface = nullptr;
}

TextServerGDNative::~TextServerGDNative() {
	if (interface != nullptr) {
		interface->destructor(data);
		data = nullptr;
		interface = nullptr;
	}
}

/*************************************************************************/
/* GDNative functions                                                    */
/*************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

static_assert(sizeof(Fox_glyph) == sizeof(TextServer::Glyph), "Glyph size mismatch");
static_assert(sizeof(Fox_packed_glyph_array) == sizeof(Vector<TextServer::Glyph>), "Vector<Glyph> size mismatch");

void GDAPI Fox_text_register_interface(const Fox_text_interface_gdnative *p_interface, const Fox_string *p_name, uint32_t p_features) {
	ERR_FAIL_COND(p_interface->version.major != 1);
	String name = *(String *)p_name;
	TextServerManager::register_create_function(name + "(GDNative)", p_features, TextServerGDNative::create_func, (void *)p_interface);
}

// Glyph

void GDAPI Fox_glyph_new(Fox_glyph *r_dest) {
	TextServer::Glyph *dest = (TextServer::Glyph *)r_dest;
	*dest = TextServer::Glyph();
}

Fox_vector2i GDAPI Fox_glyph_get_range(const Fox_glyph *p_self) {
	Fox_vector2i dest;
	Vector2i *d = (Vector2i *)&dest;
	const TextServer::Glyph *self = (const TextServer::Glyph *)p_self;
	d->x = self->start;
	d->y = self->end;
	return dest;
}

void GDAPI Fox_glyph_set_range(Fox_glyph *p_self, const Fox_vector2i *p_range) {
	TextServer::Glyph *self = (TextServer::Glyph *)p_self;
	const Vector2i *range = (const Vector2i *)p_range;
	self->start = range->x;
	self->end = range->y;
}

Fox_int GDAPI Fox_glyph_get_count(const Fox_glyph *p_self) {
	const TextServer::Glyph *self = (const TextServer::Glyph *)p_self;
	return self->count;
}

void GDAPI Fox_glyph_set_count(Fox_glyph *p_self, Fox_int p_count) {
	TextServer::Glyph *self = (TextServer::Glyph *)p_self;
	self->count = p_count;
}

Fox_int GDAPI Fox_glyph_get_repeat(const Fox_glyph *p_self) {
	const TextServer::Glyph *self = (const TextServer::Glyph *)p_self;
	return self->repeat;
}

void GDAPI Fox_glyph_set_repeat(Fox_glyph *p_self, Fox_int p_repeat) {
	TextServer::Glyph *self = (TextServer::Glyph *)p_self;
	self->repeat = p_repeat;
}

Fox_int GDAPI Fox_glyph_get_flags(const Fox_glyph *p_self) {
	const TextServer::Glyph *self = (const TextServer::Glyph *)p_self;
	return self->flags;
}

void GDAPI Fox_glyph_set_flags(Fox_glyph *p_self, Fox_int p_flags) {
	TextServer::Glyph *self = (TextServer::Glyph *)p_self;
	self->flags = p_flags;
}

Fox_vector2 GDAPI Fox_glyph_get_offset(const Fox_glyph *p_self) {
	Fox_vector2 dest;
	Vector2 *d = (Vector2 *)&dest;
	const TextServer::Glyph *self = (const TextServer::Glyph *)p_self;
	d->x = self->x_off;
	d->y = self->y_off;
	return dest;
}

void GDAPI Fox_glyph_set_offset(Fox_glyph *p_self, const Fox_vector2 *p_offset) {
	TextServer::Glyph *self = (TextServer::Glyph *)p_self;
	const Vector2 *offset = (const Vector2 *)p_offset;
	self->x_off = offset->x;
	self->y_off = offset->y;
}

Fox_float GDAPI Fox_glyph_get_advance(const Fox_glyph *p_self) {
	const TextServer::Glyph *self = (const TextServer::Glyph *)p_self;
	return self->advance;
}

void GDAPI Fox_glyph_set_advance(Fox_glyph *p_self, Fox_float p_advance) {
	TextServer::Glyph *self = (TextServer::Glyph *)p_self;
	self->advance = p_advance;
}

Fox_rid GDAPI Fox_glyph_get_font(const Fox_glyph *p_self) {
	Fox_rid dest;
	RID *d = (RID *)&dest;
	const TextServer::Glyph *self = (const TextServer::Glyph *)p_self;
	*d = self->font_rid;
	return dest;
}

void GDAPI Fox_glyph_set_font(Fox_glyph *p_self, Fox_rid *p_font) {
	TextServer::Glyph *self = (TextServer::Glyph *)p_self;
	const RID *font = (const RID *)p_font;
	self->font_rid = *font;
}

Fox_int GDAPI Fox_glyph_get_font_size(const Fox_glyph *p_self) {
	const TextServer::Glyph *self = (const TextServer::Glyph *)p_self;
	return self->font_size;
}

void GDAPI Fox_glyph_set_font_size(Fox_glyph *p_self, Fox_int p_size) {
	TextServer::Glyph *self = (TextServer::Glyph *)p_self;
	self->font_size = p_size;
}

Fox_int GDAPI Fox_glyph_get_index(const Fox_glyph *p_self) {
	const TextServer::Glyph *self = (const TextServer::Glyph *)p_self;
	return self->index;
}

void GDAPI Fox_glyph_set_index(Fox_glyph *p_self, Fox_int p_index) {
	TextServer::Glyph *self = (TextServer::Glyph *)p_self;
	self->index = p_index;
}

// GlyphArray

void GDAPI Fox_packed_glyph_array_new(Fox_packed_glyph_array *r_dest) {
	Vector<TextServer::Glyph> *dest = (Vector<TextServer::Glyph> *)r_dest;
	memnew_placement(dest, Vector<TextServer::Glyph>);
}

void GDAPI Fox_packed_glyph_array_new_copy(Fox_packed_glyph_array *r_dest, const Fox_packed_glyph_array *p_src) {
	Vector<TextServer::Glyph> *dest = (Vector<TextServer::Glyph> *)r_dest;
	const Vector<TextServer::Glyph> *src = (const Vector<TextServer::Glyph> *)p_src;
	memnew_placement(dest, Vector<TextServer::Glyph>(*src));
}

const Fox_glyph GDAPI *Fox_packed_glyph_array_ptr(const Fox_packed_glyph_array *p_self) {
	const Vector<TextServer::Glyph> *self = (const Vector<TextServer::Glyph> *)p_self;
	return (const Fox_glyph *)self->ptr();
}

Fox_glyph GDAPI *Fox_packed_glyph_array_ptrw(Fox_packed_glyph_array *p_self) {
	Vector<TextServer::Glyph> *self = (Vector<TextServer::Glyph> *)p_self;
	return (Fox_glyph *)self->ptrw();
}

void GDAPI Fox_packed_glyph_array_append(Fox_packed_glyph_array *p_self, const Fox_glyph *p_data) {
	Vector<TextServer::Glyph> *self = (Vector<TextServer::Glyph> *)p_self;
	TextServer::Glyph &s = *(TextServer::Glyph *)p_data;
	self->push_back(s);
}

void GDAPI Fox_packed_glyph_array_append_array(Fox_packed_glyph_array *p_self, const Fox_packed_glyph_array *p_array) {
	Vector<TextServer::Glyph> *self = (Vector<TextServer::Glyph> *)p_self;
	Vector<TextServer::Glyph> *array = (Vector<TextServer::Glyph> *)p_array;
	self->append_array(*array);
}

Fox_error GDAPI Fox_packed_glyph_array_insert(Fox_packed_glyph_array *p_self, const Fox_int p_idx, const Fox_glyph *p_data) {
	Vector<TextServer::Glyph> *self = (Vector<TextServer::Glyph> *)p_self;
	TextServer::Glyph &s = *(TextServer::Glyph *)p_data;
	return (Fox_error)self->insert(p_idx, s);
}

Fox_bool GDAPI Fox_packed_glyph_array_has(Fox_packed_glyph_array *p_self, const Fox_glyph *p_value) {
	Vector<TextServer::Glyph> *self = (Vector<TextServer::Glyph> *)p_self;
	TextServer::Glyph &v = *(TextServer::Glyph *)p_value;
	return (Fox_bool)self->has(v);
}

void GDAPI Fox_packed_glyph_array_sort(Fox_packed_glyph_array *p_self) {
	Vector<TextServer::Glyph> *self = (Vector<TextServer::Glyph> *)p_self;
	self->sort();
}

void GDAPI Fox_packed_glyph_array_reverse(Fox_packed_glyph_array *p_self) {
	Vector<TextServer::Glyph> *self = (Vector<TextServer::Glyph> *)p_self;
	self->reverse();
}

void GDAPI Fox_packed_glyph_array_push_back(Fox_packed_glyph_array *p_self, const Fox_glyph *p_data) {
	Vector<TextServer::Glyph> *self = (Vector<TextServer::Glyph> *)p_self;
	TextServer::Glyph &s = *(TextServer::Glyph *)p_data;
	self->push_back(s);
}

void GDAPI Fox_packed_glyph_array_remove(Fox_packed_glyph_array *p_self, const Fox_int p_idx) {
	Vector<TextServer::Glyph> *self = (Vector<TextServer::Glyph> *)p_self;
	self->remove(p_idx);
}

void GDAPI Fox_packed_glyph_array_resize(Fox_packed_glyph_array *p_self, const Fox_int p_size) {
	Vector<TextServer::Glyph> *self = (Vector<TextServer::Glyph> *)p_self;
	self->resize(p_size);
}

void GDAPI Fox_packed_glyph_array_set(Fox_packed_glyph_array *p_self, const Fox_int p_idx, const Fox_glyph *p_data) {
	Vector<TextServer::Glyph> *self = (Vector<TextServer::Glyph> *)p_self;
	TextServer::Glyph &s = *(TextServer::Glyph *)p_data;
	self->set(p_idx, s);
}

Fox_glyph GDAPI Fox_packed_glyph_array_get(const Fox_packed_glyph_array *p_self, const Fox_int p_idx) {
	const Vector<TextServer::Glyph> *self = (const Vector<TextServer::Glyph> *)p_self;
	Fox_glyph v;
	TextServer::Glyph *s = (TextServer::Glyph *)&v;
	*s = self->get(p_idx);
	return v;
}

Fox_int GDAPI Fox_packed_glyph_array_size(const Fox_packed_glyph_array *p_self) {
	const Vector<TextServer::Glyph> *self = (const Vector<TextServer::Glyph> *)p_self;
	return self->size();
}

Fox_bool GDAPI Fox_packed_glyph_array_is_empty(const Fox_packed_glyph_array *p_self) {
	const Vector<TextServer::Glyph> *self = (const Vector<TextServer::Glyph> *)p_self;
	return self->is_empty();
}

void GDAPI Fox_packed_glyph_array_destroy(Fox_packed_glyph_array *p_self) {
	((Vector<TextServer::Glyph> *)p_self)->~Vector();
}

#ifdef __cplusplus
}
#endif
