/*************************************************************************/
/*  height_map_shape_3d.h                                                */
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

#ifndef HEIGHT_MAP_SHAPE_H
#define HEIGHT_MAP_SHAPE_H

#include "scene/resources/shape_3d.h"

class HeightMapShape3D : public Shape3D {
	GDCLASS(HeightMapShape3D, Shape3D);

	int map_width = 2;
	int map_depth = 2;
	PackedFloat32Array map_data;
	real_t min_height = 0.0;
	real_t max_height = 0.0;

protected:
	static void _bind_methods();
	virtual void _update_shape() override;

public:
	void set_map_width(int p_new);
	int get_map_width() const;
	void set_map_depth(int p_new);
	int get_map_depth() const;
	void set_map_data(PackedFloat32Array p_new);
	PackedFloat32Array get_map_data() const;

	virtual Vector<Vector3> get_debug_mesh_lines() const override;
	virtual real_t get_enclosing_radius() const override;

	HeightMapShape3D();
};

#endif /* !HEIGHT_MAP_SHAPE_H */
