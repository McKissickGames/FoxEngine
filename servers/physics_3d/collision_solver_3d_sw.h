/*************************************************************************/
/*  collision_solver_3d_sw.h                                             */
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

#ifndef COLLISION_SOLVER_SW_H
#define COLLISION_SOLVER_SW_H

#include "shape_3d_sw.h"

class CollisionSolver3DSW {
public:
	typedef void (*CallbackResult)(const Vector3 &p_point_A, int p_index_A, const Vector3 &p_point_B, int p_index_B, void *p_userdata);

private:
	static bool soft_body_query_callback(uint32_t p_node_index, void *p_userdata);
	static void soft_body_contact_callback(const Vector3 &p_point_A, int p_index_A, const Vector3 &p_point_B, int p_index_B, void *p_userdata);
	static void soft_body_concave_callback(void *p_userdata, Shape3DSW *p_convex);
	static void concave_callback(void *p_userdata, Shape3DSW *p_convex);
	static bool solve_static_plane(const Shape3DSW *p_shape_A, const Transform3D &p_transform_A, const Shape3DSW *p_shape_B, const Transform3D &p_transform_B, CallbackResult p_result_callback, void *p_userdata, bool p_swap_result);
	static bool solve_ray(const Shape3DSW *p_shape_A, const Transform3D &p_transform_A, const Shape3DSW *p_shape_B, const Transform3D &p_transform_B, CallbackResult p_result_callback, void *p_userdata, bool p_swap_result);
	static bool solve_soft_body(const Shape3DSW *p_shape_A, const Transform3D &p_transform_A, const Shape3DSW *p_shape_B, const Transform3D &p_transform_B, CallbackResult p_result_callback, void *p_userdata, bool p_swap_result);
	static bool solve_concave(const Shape3DSW *p_shape_A, const Transform3D &p_transform_A, const Shape3DSW *p_shape_B, const Transform3D &p_transform_B, CallbackResult p_result_callback, void *p_userdata, bool p_swap_result, real_t p_margin_A = 0, real_t p_margin_B = 0);
	static void concave_distance_callback(void *p_userdata, Shape3DSW *p_convex);
	static bool solve_distance_plane(const Shape3DSW *p_shape_A, const Transform3D &p_transform_A, const Shape3DSW *p_shape_B, const Transform3D &p_transform_B, Vector3 &r_point_A, Vector3 &r_point_B);

public:
	static bool solve_static(const Shape3DSW *p_shape_A, const Transform3D &p_transform_A, const Shape3DSW *p_shape_B, const Transform3D &p_transform_B, CallbackResult p_result_callback, void *p_userdata, Vector3 *r_sep_axis = nullptr, real_t p_margin_A = 0, real_t p_margin_B = 0);
	static bool solve_distance(const Shape3DSW *p_shape_A, const Transform3D &p_transform_A, const Shape3DSW *p_shape_B, const Transform3D &p_transform_B, Vector3 &r_point_A, Vector3 &r_point_B, const AABB &p_concave_hint, Vector3 *r_sep_axis = nullptr);
};

#endif // COLLISION_SOLVER__SW_H
