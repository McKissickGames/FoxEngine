/*************************************************************************/
/*  font_adv.h                                                           */
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

#ifndef FONT_ADV_H
#define FONT_ADV_H

#include "servers/text_server.h"

#include <hb.h>

struct FontDataAdvanced {
	Map<String, bool> lang_support_overrides;
	Map<String, bool> script_support_overrides;
	bool valid = false;
	int spacing_space = 0;
	int spacing_glyph = 0;

	virtual void clear_cache() = 0;

	virtual Error load_from_file(const String &p_filename, int p_base_size) { return ERR_CANT_CREATE; };
	virtual Error load_from_memory(const uint8_t *p_data, size_t p_size, int p_base_size) { return ERR_CANT_CREATE; };
	virtual Error bitmap_new(float p_height, float p_ascent, int p_base_size) { return ERR_CANT_CREATE; };

	virtual void bitmap_add_texture(const Ref<Texture> &p_texture) { ERR_FAIL_MSG("Not supported."); };
	virtual void bitmap_add_char(char32_t p_char, int p_texture_idx, const Rect2 &p_rect, const Size2 &p_align, float p_advance) { ERR_FAIL_MSG("Not supported."); };
	virtual void bitmap_add_kerning_pair(char32_t p_A, char32_t p_B, int p_kerning) { ERR_FAIL_MSG("Not supported."); };

	virtual float get_height(int p_size) const = 0;
	virtual float get_ascent(int p_size) const = 0;
	virtual float get_descent(int p_size) const = 0;

	virtual Dictionary get_feature_list() const { return Dictionary(); };
	virtual Dictionary get_variation_list() const { return Dictionary(); };

	virtual void set_variation(const String &p_name, double p_value){};
	virtual double get_variation(const String &p_name) const { return 0; };

	virtual float get_underline_position(int p_size) const = 0;
	virtual float get_underline_thickness(int p_size) const = 0;

	virtual int get_spacing_space() const { return spacing_space; };
	virtual void set_spacing_space(int p_value) {
		spacing_space = p_value;
		clear_cache();
	};

	virtual int get_spacing_glyph() const { return spacing_glyph; };
	virtual void set_spacing_glyph(int p_value) {
		spacing_glyph = p_value;
		clear_cache();
	};

	virtual void set_antialiased(bool p_antialiased) = 0;
	virtual bool get_antialiased() const = 0;

	virtual void set_hinting(TextServer::Hinting p_hinting) = 0;
	virtual TextServer::Hinting get_hinting() const = 0;

	virtual void set_distance_field_hint(bool p_distance_field) = 0;
	virtual bool get_distance_field_hint() const = 0;

	virtual void set_force_autohinter(bool p_enabeld) = 0;
	virtual bool get_force_autohinter() const = 0;

	virtual bool has_outline() const = 0;
	virtual float get_base_size() const = 0;

	virtual bool is_lang_supported(const String &p_lang) const { return true; };
	virtual bool is_script_supported(uint32_t p_script) const { return true; };

	virtual bool has_char(char32_t p_char) const = 0;
	virtual String get_supported_chars() const = 0;
	virtual float get_font_scale(int p_size) const { return 1.0f; };

	virtual hb_font_t *get_hb_handle(int p_size) = 0;
	virtual uint32_t get_glyph_index(char32_t p_char, char32_t p_variation_selector) const = 0;
	virtual Vector2 get_advance(uint32_t p_char, int p_size) const = 0;
	virtual Vector2 get_kerning(uint32_t p_char, uint32_t p_next, int p_size) const = 0;

	virtual Vector2 draw_glyph(RID p_canvas, int p_size, const Vector2 &p_pos, uint32_t p_index, const Color &p_color) const = 0;
	virtual Vector2 draw_glyph_outline(RID p_canvas, int p_size, int p_outline_size, const Vector2 &p_pos, uint32_t p_index, const Color &p_color) const = 0;

	virtual bool get_glyph_contours(int p_size, uint32_t p_index, Vector<Vector3> &r_points, Vector<int32_t> &r_contours, bool &r_orientation) const { return false; };

	virtual ~FontDataAdvanced(){};
};

#endif // FONT_ADV_H
