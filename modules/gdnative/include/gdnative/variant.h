/*************************************************************************/
/*  variant.h                                                            */
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

#ifndef Fox_VARIANT_H
#define Fox_VARIANT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <gdnative/math_defs.h>
#include <gdnative/variant_struct.h>

typedef enum Fox_variant_type {
	Fox_VARIANT_TYPE_NIL,

	// atomic types
	Fox_VARIANT_TYPE_BOOL,
	Fox_VARIANT_TYPE_INT,
	Fox_VARIANT_TYPE_FLOAT,
	Fox_VARIANT_TYPE_STRING,

	// math types
	Fox_VARIANT_TYPE_VECTOR2,
	Fox_VARIANT_TYPE_VECTOR2I,
	Fox_VARIANT_TYPE_RECT2,
	Fox_VARIANT_TYPE_RECT2I,
	Fox_VARIANT_TYPE_VECTOR3,
	Fox_VARIANT_TYPE_VECTOR3I,
	Fox_VARIANT_TYPE_TRANSFORM2D,
	Fox_VARIANT_TYPE_PLANE,
	Fox_VARIANT_TYPE_QUATERNION,
	Fox_VARIANT_TYPE_AABB,
	Fox_VARIANT_TYPE_BASIS,
	Fox_VARIANT_TYPE_TRANSFORM3D,

	// misc types
	Fox_VARIANT_TYPE_COLOR,
	Fox_VARIANT_TYPE_STRING_NAME,
	Fox_VARIANT_TYPE_NODE_PATH,
	Fox_VARIANT_TYPE_RID,
	Fox_VARIANT_TYPE_OBJECT,
	Fox_VARIANT_TYPE_CALLABLE,
	Fox_VARIANT_TYPE_SIGNAL,
	Fox_VARIANT_TYPE_DICTIONARY,
	Fox_VARIANT_TYPE_ARRAY,

	// arrays
	Fox_VARIANT_TYPE_PACKED_BYTE_ARRAY,
	Fox_VARIANT_TYPE_PACKED_INT32_ARRAY,
	Fox_VARIANT_TYPE_PACKED_INT64_ARRAY,
	Fox_VARIANT_TYPE_PACKED_FLOAT32_ARRAY,
	Fox_VARIANT_TYPE_PACKED_FLOAT64_ARRAY,
	Fox_VARIANT_TYPE_PACKED_STRING_ARRAY,
	Fox_VARIANT_TYPE_PACKED_VECTOR2_ARRAY,
	Fox_VARIANT_TYPE_PACKED_VECTOR3_ARRAY,
	Fox_VARIANT_TYPE_PACKED_COLOR_ARRAY,
} Fox_variant_type;

typedef enum Fox_variant_call_error_error {
	Fox_CALL_ERROR_CALL_OK,
	Fox_CALL_ERROR_CALL_ERROR_INVALID_METHOD,
	Fox_CALL_ERROR_CALL_ERROR_INVALID_ARGUMENT,
	Fox_CALL_ERROR_CALL_ERROR_TOO_MANY_ARGUMENTS,
	Fox_CALL_ERROR_CALL_ERROR_TOO_FEW_ARGUMENTS,
	Fox_CALL_ERROR_CALL_ERROR_INSTANCE_IS_NULL,
} Fox_variant_call_error_error;

typedef struct Fox_variant_call_error {
	Fox_variant_call_error_error error;
	int argument;
	Fox_variant_type expected;
} Fox_variant_call_error;

typedef enum Fox_variant_operator {
	// comparison
	Fox_VARIANT_OP_EQUAL,
	Fox_VARIANT_OP_NOT_EQUAL,
	Fox_VARIANT_OP_LESS,
	Fox_VARIANT_OP_LESS_EQUAL,
	Fox_VARIANT_OP_GREATER,
	Fox_VARIANT_OP_GREATER_EQUAL,

	// mathematic
	Fox_VARIANT_OP_ADD,
	Fox_VARIANT_OP_SUBTRACT,
	Fox_VARIANT_OP_MULTIPLY,
	Fox_VARIANT_OP_DIVIDE,
	Fox_VARIANT_OP_NEGATE,
	Fox_VARIANT_OP_POSITIVE,
	Fox_VARIANT_OP_MODULE,
	Fox_VARIANT_OP_STRING_CONCAT,

	// bitwise
	Fox_VARIANT_OP_SHIFT_LEFT,
	Fox_VARIANT_OP_SHIFT_RIGHT,
	Fox_VARIANT_OP_BIT_AND,
	Fox_VARIANT_OP_BIT_OR,
	Fox_VARIANT_OP_BIT_XOR,
	Fox_VARIANT_OP_BIT_NEGATE,

	// logic
	Fox_VARIANT_OP_AND,
	Fox_VARIANT_OP_OR,
	Fox_VARIANT_OP_XOR,
	Fox_VARIANT_OP_NOT,

	// containment
	Fox_VARIANT_OP_IN,

	Fox_VARIANT_OP_MAX,
} Fox_variant_operator;

typedef enum Fox_variant_utility_function_type {
	Fox_UTILITY_FUNC_TYPE_MATH,
	Fox_UTILITY_FUNC_TYPE_RANDOM,
	Fox_UTILITY_FUNC_TYPE_GENERAL,
} Fox_variant_utility_function_type;

// Types for function pointers.
typedef void (*Fox_validated_operator_evaluator)(const Fox_variant *p_left, const Fox_variant *p_right, Fox_variant *r_result);
typedef void (*Fox_ptr_operator_evaluator)(const void *p_left, const void *p_right, void *r_result);
typedef void (*Fox_validated_builtin_method)(Fox_variant *p_base, const Fox_variant **p_args, int p_argument_count, Fox_variant *r_return);
typedef void (*Fox_ptr_builtin_method)(void *p_base, const void **p_args, void *r_return, int p_argument_count);
typedef void (*Fox_validated_constructor)(Fox_variant *p_base, const Fox_variant **p_args);
typedef void (*Fox_ptr_constructor)(void *p_base, const void **p_args);
typedef void (*Fox_validated_setter)(Fox_variant *p_base, const Fox_variant *p_value);
typedef void (*Fox_validated_getter)(const Fox_variant *p_base, Fox_variant *r_value);
typedef void (*Fox_ptr_setter)(void *p_base, const void *p_value);
typedef void (*Fox_ptr_getter)(const void *p_base, void *r_value);
typedef void (*Fox_validated_indexed_setter)(Fox_variant *p_base, Fox_int p_index, const Fox_variant *p_value, bool *r_oob);
typedef void (*Fox_validated_indexed_getter)(const Fox_variant *p_base, Fox_int p_index, Fox_variant *r_value, bool *r_oob);
typedef void (*Fox_ptr_indexed_setter)(void *p_base, Fox_int p_index, const void *p_value);
typedef void (*Fox_ptr_indexed_getter)(const void *p_base, Fox_int p_index, void *r_value);
typedef void (*Fox_validated_keyed_setter)(Fox_variant *p_base, const Fox_variant *p_key, const Fox_variant *p_value, bool *r_valid);
typedef void (*Fox_validated_keyed_getter)(const Fox_variant *p_base, const Fox_variant *p_key, Fox_variant *r_value, bool *r_valid);
typedef bool (*Fox_validated_keyed_checker)(const Fox_variant *p_base, const Fox_variant *p_key, bool *r_valid);
typedef void (*Fox_ptr_keyed_setter)(void *p_base, const void *p_key, const void *p_value);
typedef void (*Fox_ptr_keyed_getter)(const void *p_base, const void *p_key, void *r_value);
typedef uint32_t (*Fox_ptr_keyed_checker)(const Fox_variant *p_base, const Fox_variant *p_key);
typedef void (*Fox_validated_utility_function)(Fox_variant *r_return, const Fox_variant **p_arguments, int p_argument_count);
typedef void (*Fox_ptr_utility_function)(void *r_return, const void **p_arguments, int p_argument_count);

#include <gdnative/aabb.h>
#include <gdnative/array.h>
#include <gdnative/basis.h>
#include <gdnative/callable.h>
#include <gdnative/color.h>
#include <gdnative/dictionary.h>
#include <gdnative/node_path.h>
#include <gdnative/packed_arrays.h>
#include <gdnative/plane.h>
#include <gdnative/quaternion.h>
#include <gdnative/rect2.h>
#include <gdnative/rid.h>
#include <gdnative/signal.h>
#include <gdnative/string.h>
#include <gdnative/string_name.h>
#include <gdnative/transform2d.h>
#include <gdnative/transform_3d.h>
#include <gdnative/variant.h>
#include <gdnative/vector2.h>
#include <gdnative/vector3.h>

#include <gdnative/gdnative.h>

// Memory.

void GDAPI Fox_variant_new_copy(Fox_variant *r_dest, const Fox_variant *p_src);

void GDAPI Fox_variant_new_nil(Fox_variant *r_dest);
void GDAPI Fox_variant_new_bool(Fox_variant *r_dest, const Fox_bool p_b);
void GDAPI Fox_variant_new_int(Fox_variant *r_dest, const Fox_int p_i);
void GDAPI Fox_variant_new_float(Fox_variant *r_dest, const Fox_float p_f);
void GDAPI Fox_variant_new_string(Fox_variant *r_dest, const Fox_string *p_s);
void GDAPI Fox_variant_new_vector2(Fox_variant *r_dest, const Fox_vector2 *p_v2);
void GDAPI Fox_variant_new_vector2i(Fox_variant *r_dest, const Fox_vector2i *p_v2);
void GDAPI Fox_variant_new_rect2(Fox_variant *r_dest, const Fox_rect2 *p_rect2);
void GDAPI Fox_variant_new_rect2i(Fox_variant *r_dest, const Fox_rect2i *p_rect2);
void GDAPI Fox_variant_new_vector3(Fox_variant *r_dest, const Fox_vector3 *p_v3);
void GDAPI Fox_variant_new_vector3i(Fox_variant *r_dest, const Fox_vector3i *p_v3);
void GDAPI Fox_variant_new_transform2d(Fox_variant *r_dest, const Fox_transform2d *p_t2d);
void GDAPI Fox_variant_new_plane(Fox_variant *r_dest, const Fox_plane *p_plane);
void GDAPI Fox_variant_new_quaternion(Fox_variant *r_dest, const Fox_quaternion *p_quaternion);
void GDAPI Fox_variant_new_aabb(Fox_variant *r_dest, const Fox_aabb *p_aabb);
void GDAPI Fox_variant_new_basis(Fox_variant *r_dest, const Fox_basis *p_basis);
void GDAPI Fox_variant_new_transform3d(Fox_variant *r_dest, const Fox_transform3d *p_trans);
void GDAPI Fox_variant_new_color(Fox_variant *r_dest, const Fox_color *p_color);
void GDAPI Fox_variant_new_string_name(Fox_variant *r_dest, const Fox_string_name *p_s);
void GDAPI Fox_variant_new_node_path(Fox_variant *r_dest, const Fox_node_path *p_np);
void GDAPI Fox_variant_new_rid(Fox_variant *r_dest, const Fox_rid *p_rid);
void GDAPI Fox_variant_new_object(Fox_variant *r_dest, const Fox_object *p_obj);
void GDAPI Fox_variant_new_callable(Fox_variant *r_dest, const Fox_callable *p_callable);
void GDAPI Fox_variant_new_signal(Fox_variant *r_dest, const Fox_signal *p_signal);
void GDAPI Fox_variant_new_dictionary(Fox_variant *r_dest, const Fox_dictionary *p_dict);
void GDAPI Fox_variant_new_array(Fox_variant *r_dest, const Fox_array *p_arr);
void GDAPI Fox_variant_new_packed_byte_array(Fox_variant *r_dest, const Fox_packed_byte_array *p_pba);
void GDAPI Fox_variant_new_packed_int32_array(Fox_variant *r_dest, const Fox_packed_int32_array *p_pia);
void GDAPI Fox_variant_new_packed_int64_array(Fox_variant *r_dest, const Fox_packed_int64_array *p_pia);
void GDAPI Fox_variant_new_packed_float32_array(Fox_variant *r_dest, const Fox_packed_float32_array *p_pra);
void GDAPI Fox_variant_new_packed_float64_array(Fox_variant *r_dest, const Fox_packed_float64_array *p_pra);
void GDAPI Fox_variant_new_packed_string_array(Fox_variant *r_dest, const Fox_packed_string_array *p_psa);
void GDAPI Fox_variant_new_packed_vector2_array(Fox_variant *r_dest, const Fox_packed_vector2_array *p_pv2a);
void GDAPI Fox_variant_new_packed_vector3_array(Fox_variant *r_dest, const Fox_packed_vector3_array *p_pv3a);
void GDAPI Fox_variant_new_packed_color_array(Fox_variant *r_dest, const Fox_packed_color_array *p_pca);

Fox_bool GDAPI Fox_variant_as_bool(const Fox_variant *p_self);
Fox_int GDAPI Fox_variant_as_int(const Fox_variant *p_self);
Fox_float GDAPI Fox_variant_as_float(const Fox_variant *p_self);
Fox_string GDAPI Fox_variant_as_string(const Fox_variant *p_self);
Fox_vector2 GDAPI Fox_variant_as_vector2(const Fox_variant *p_self);
Fox_vector2i GDAPI Fox_variant_as_vector2i(const Fox_variant *p_self);
Fox_rect2 GDAPI Fox_variant_as_rect2(const Fox_variant *p_self);
Fox_rect2i GDAPI Fox_variant_as_rect2i(const Fox_variant *p_self);
Fox_vector3 GDAPI Fox_variant_as_vector3(const Fox_variant *p_self);
Fox_vector3i GDAPI Fox_variant_as_vector3i(const Fox_variant *p_self);
Fox_transform2d GDAPI Fox_variant_as_transform2d(const Fox_variant *p_self);
Fox_plane GDAPI Fox_variant_as_plane(const Fox_variant *p_self);
Fox_quaternion GDAPI Fox_variant_as_quaternion(const Fox_variant *p_self);
Fox_aabb GDAPI Fox_variant_as_aabb(const Fox_variant *p_self);
Fox_basis GDAPI Fox_variant_as_basis(const Fox_variant *p_self);
Fox_transform3d GDAPI Fox_variant_as_transform3d(const Fox_variant *p_self);
Fox_color GDAPI Fox_variant_as_color(const Fox_variant *p_self);
Fox_string_name GDAPI Fox_variant_as_string_name(const Fox_variant *p_self);
Fox_node_path GDAPI Fox_variant_as_node_path(const Fox_variant *p_self);
Fox_rid GDAPI Fox_variant_as_rid(const Fox_variant *p_self);
Fox_object GDAPI *Fox_variant_as_object(const Fox_variant *p_self);
Fox_callable GDAPI Fox_variant_as_callable(const Fox_variant *p_self);
Fox_signal GDAPI Fox_variant_as_signal(const Fox_variant *p_self);
Fox_dictionary GDAPI Fox_variant_as_dictionary(const Fox_variant *p_self);
Fox_array GDAPI Fox_variant_as_array(const Fox_variant *p_self);
Fox_packed_byte_array GDAPI Fox_variant_as_packed_byte_array(const Fox_variant *p_self);
Fox_packed_int32_array GDAPI Fox_variant_as_packed_int32_array(const Fox_variant *p_self);
Fox_packed_int64_array GDAPI Fox_variant_as_packed_int64_array(const Fox_variant *p_self);
Fox_packed_float32_array GDAPI Fox_variant_as_packed_float32_array(const Fox_variant *p_self);
Fox_packed_float64_array GDAPI Fox_variant_as_packed_float64_array(const Fox_variant *p_self);
Fox_packed_string_array GDAPI Fox_variant_as_packed_string_array(const Fox_variant *p_self);
Fox_packed_vector2_array GDAPI Fox_variant_as_packed_vector2_array(const Fox_variant *p_self);
Fox_packed_vector3_array GDAPI Fox_variant_as_packed_vector3_array(const Fox_variant *p_self);
Fox_packed_color_array GDAPI Fox_variant_as_packed_color_array(const Fox_variant *p_self);

void GDAPI Fox_variant_destroy(Fox_variant *p_self);

// Dynamic interaction.

void GDAPI Fox_variant_call(Fox_variant *p_self, const Fox_string_name *p_method, const Fox_variant **p_args, const Fox_int p_argument_count, Fox_variant *r_return, Fox_variant_call_error *r_error);
void GDAPI Fox_variant_call_with_cstring(Fox_variant *p_self, const char *p_method, const Fox_variant **p_args, const Fox_int p_argument_count, Fox_variant *r_return, Fox_variant_call_error *r_error);
void GDAPI Fox_variant_call_static(Fox_variant_type p_type, const Fox_string_name *p_method, const Fox_variant **p_args, const Fox_int p_argument_count, Fox_variant *r_return, Fox_variant_call_error *r_error);
void GDAPI Fox_variant_call_static_with_cstring(Fox_variant_type p_type, const char *p_method, const Fox_variant **p_args, const Fox_int p_argument_count, Fox_variant *r_return, Fox_variant_call_error *r_error);
void GDAPI Fox_variant_evaluate(Fox_variant_operator p_op, const Fox_variant *p_a, const Fox_variant *p_b, Fox_variant *r_return, bool *r_valid);
void GDAPI Fox_variant_set(Fox_variant *p_self, const Fox_variant *p_key, const Fox_variant *p_value, bool *r_valid);
void GDAPI Fox_variant_set_named(Fox_variant *p_self, const Fox_string_name *p_name, const Fox_variant *p_value, bool *r_valid);
void GDAPI Fox_variant_set_named_with_cstring(Fox_variant *p_self, const char *p_name, const Fox_variant *p_value, bool *r_valid);
void GDAPI Fox_variant_set_keyed(Fox_variant *p_self, const Fox_variant *p_key, const Fox_variant *p_value, bool *r_valid);
void GDAPI Fox_variant_set_indexed(Fox_variant *p_self, Fox_int p_index, const Fox_variant *p_value, bool *r_valid, bool *r_oob);
Fox_variant GDAPI Fox_variant_get(const Fox_variant *p_self, const Fox_variant *p_key, bool *r_valid);
Fox_variant GDAPI Fox_variant_get_named(const Fox_variant *p_self, const Fox_string_name *p_key, bool *r_valid);
Fox_variant GDAPI Fox_variant_get_named_with_cstring(const Fox_variant *p_self, const char *p_key, bool *r_valid);
Fox_variant GDAPI Fox_variant_get_keyed(const Fox_variant *p_self, const Fox_variant *p_key, bool *r_valid);
Fox_variant GDAPI Fox_variant_get_indexed(const Fox_variant *p_self, Fox_int p_index, bool *r_valid, bool *r_oob);
/// Iteration.
bool GDAPI Fox_variant_iter_init(const Fox_variant *p_self, Fox_variant *r_iter, bool *r_valid);
bool GDAPI Fox_variant_iter_next(const Fox_variant *p_self, Fox_variant *r_iter, bool *r_valid);
Fox_variant GDAPI Fox_variant_iter_get(const Fox_variant *p_self, Fox_variant *r_iter, bool *r_valid);

/// Variant functions.
Fox_bool GDAPI Fox_variant_hash_compare(const Fox_variant *p_self, const Fox_variant *p_other);
Fox_bool GDAPI Fox_variant_booleanize(const Fox_variant *p_self);
void GDAPI Fox_variant_blend(const Fox_variant *p_a, const Fox_variant *p_b, float p_c, Fox_variant *r_dst);
void GDAPI Fox_variant_interpolate(const Fox_variant *p_a, const Fox_variant *p_b, float p_c, Fox_variant *r_dst);
Fox_variant GDAPI Fox_variant_duplicate(const Fox_variant *p_self, Fox_bool p_deep);
Fox_string GDAPI Fox_variant_stringify(const Fox_variant *p_self);

// Discovery API.

/// Operators.
Fox_validated_operator_evaluator GDAPI Fox_variant_get_validated_operator_evaluator(Fox_variant_operator p_operator, Fox_variant_type p_type_a, Fox_variant_type p_type_b);
Fox_ptr_operator_evaluator GDAPI Fox_variant_get_ptr_operator_evaluator(Fox_variant_operator p_operator, Fox_variant_type p_type_a, Fox_variant_type p_type_b);
Fox_variant_type GDAPI Fox_variant_get_operator_return_type(Fox_variant_operator p_operator, Fox_variant_type p_type_a, Fox_variant_type p_type_b);
Fox_string GDAPI Fox_variant_get_operator_name(Fox_variant_operator p_operator);

/// Built-in methods.
bool GDAPI Fox_variant_has_builtin_method(Fox_variant_type p_type, const Fox_string_name *p_method);
bool GDAPI Fox_variant_has_builtin_method_with_cstring(Fox_variant_type p_type, const char *p_method);
Fox_validated_builtin_method GDAPI Fox_variant_get_validated_builtin_method(Fox_variant_type p_type, const Fox_string_name *p_method);
Fox_validated_builtin_method GDAPI Fox_variant_get_validated_builtin_method_with_cstring(Fox_variant_type p_type, const char *p_method);
Fox_ptr_builtin_method GDAPI Fox_variant_get_ptr_builtin_method(Fox_variant_type p_type, const Fox_string_name *p_method);
Fox_ptr_builtin_method GDAPI Fox_variant_get_ptr_builtin_method_with_cstring(Fox_variant_type p_type, const char *p_method);
int GDAPI Fox_variant_get_builtin_method_argument_count(Fox_variant_type p_type, const Fox_string_name *p_method);
int GDAPI Fox_variant_get_builtin_method_argument_count_with_cstring(Fox_variant_type p_type, const char *p_method);
Fox_variant_type GDAPI Fox_variant_get_builtin_method_argument_type(Fox_variant_type p_type, const Fox_string_name *p_method, int p_argument);
Fox_variant_type GDAPI Fox_variant_get_builtin_method_argument_type_with_cstring(Fox_variant_type p_type, const char *p_method, int p_argument);
Fox_string GDAPI Fox_variant_get_builtin_method_argument_name(Fox_variant_type p_type, const Fox_string_name *p_method, int p_argument);
Fox_string GDAPI Fox_variant_get_builtin_method_argument_name_with_cstring(Fox_variant_type p_type, const char *p_method, int p_argument);
bool GDAPI Fox_variant_has_builtin_method_return_value(Fox_variant_type p_type, const Fox_string_name *p_method);
bool GDAPI Fox_variant_has_builtin_method_return_value_with_cstring(Fox_variant_type p_type, const char *p_method);
Fox_variant_type GDAPI Fox_variant_get_builtin_method_return_type(Fox_variant_type p_type, const Fox_string_name *p_method);
Fox_variant_type GDAPI Fox_variant_get_builtin_method_return_type_with_cstring(Fox_variant_type p_type, const char *p_method);
bool GDAPI Fox_variant_is_builtin_method_const(Fox_variant_type p_type, const Fox_string_name *p_method);
bool GDAPI Fox_variant_is_builtin_method_const_with_cstring(Fox_variant_type p_type, const char *p_method);
bool GDAPI Fox_variant_is_builtin_method_static(Fox_variant_type p_type, const Fox_string_name *p_method);
bool GDAPI Fox_variant_is_builtin_method_static_with_cstring(Fox_variant_type p_type, const char *p_method);
bool GDAPI Fox_variant_is_builtin_method_vararg(Fox_variant_type p_type, const Fox_string_name *p_method);
bool GDAPI Fox_variant_is_builtin_method_vararg_with_cstring(Fox_variant_type p_type, const char *p_method);
int GDAPI Fox_variant_get_builtin_method_count(Fox_variant_type p_type);
void GDAPI Fox_variant_get_builtin_method_list(Fox_variant_type p_type, Fox_string_name *r_list);

/// Constructors.
int GDAPI Fox_variant_get_constructor_count(Fox_variant_type p_type);
Fox_validated_constructor GDAPI Fox_variant_get_validated_constructor(Fox_variant_type p_type, int p_constructor);
Fox_ptr_constructor GDAPI Fox_variant_get_ptr_constructor(Fox_variant_type p_type, int p_constructor);
int GDAPI Fox_variant_get_constructor_argument_count(Fox_variant_type p_type, int p_constructor);
Fox_variant_type GDAPI Fox_variant_get_constructor_argument_type(Fox_variant_type p_type, int p_constructor, int p_argument);
Fox_string GDAPI Fox_variant_get_constructor_argument_name(Fox_variant_type p_type, int p_constructor, int p_argument);
void GDAPI Fox_variant_construct(Fox_variant_type p_type, Fox_variant *p_base, const Fox_variant **p_args, int p_argument_count, Fox_variant_call_error *r_error);

/// Properties.
Fox_variant_type GDAPI Fox_variant_get_member_type(Fox_variant_type p_type, const Fox_string_name *p_member);
Fox_variant_type GDAPI Fox_variant_get_member_type_with_cstring(Fox_variant_type p_type, const char *p_member);
int GDAPI Fox_variant_get_member_count(Fox_variant_type p_type);
void GDAPI Fox_variant_get_member_list(Fox_variant_type p_type, Fox_string_name *r_list);
Fox_validated_setter GDAPI Fox_variant_get_validated_setter(Fox_variant_type p_type, const Fox_string_name *p_member);
Fox_validated_setter GDAPI Fox_variant_get_validated_setter_with_cstring(Fox_variant_type p_type, const char *p_member);
Fox_validated_getter GDAPI Fox_variant_get_validated_getter(Fox_variant_type p_type, const Fox_string_name *p_member);
Fox_validated_getter GDAPI Fox_variant_get_validated_getter_with_cstring(Fox_variant_type p_type, const char *p_member);
Fox_ptr_setter GDAPI Fox_variant_get_ptr_setter(Fox_variant_type p_type, const Fox_string_name *p_member);
Fox_ptr_setter GDAPI Fox_variant_get_ptr_setter_with_cstring(Fox_variant_type p_type, const char *p_member);
Fox_ptr_getter GDAPI Fox_variant_get_ptr_getter(Fox_variant_type p_type, const Fox_string_name *p_member);
Fox_ptr_getter GDAPI Fox_variant_get_ptr_getter_with_cstring(Fox_variant_type p_type, const char *p_member);

/// Indexing.
bool GDAPI Fox_variant_has_indexing(Fox_variant_type p_type);
Fox_variant_type GDAPI Fox_variant_get_indexed_element_type(Fox_variant_type p_type);
Fox_validated_indexed_setter GDAPI Fox_variant_get_validated_indexed_setter(Fox_variant_type p_type);
Fox_validated_indexed_getter GDAPI Fox_variant_get_validated_indexed_getter(Fox_variant_type p_type);
Fox_ptr_indexed_setter GDAPI Fox_variant_get_ptr_indexed_setter(Fox_variant_type p_type);
Fox_ptr_indexed_getter GDAPI Fox_variant_get_ptr_indexed_getter(Fox_variant_type p_type);
uint64_t GDAPI Fox_variant_get_indexed_size(const Fox_variant *p_self);

/// Keying.
bool GDAPI Fox_variant_is_keyed(Fox_variant_type p_type);
Fox_validated_keyed_setter GDAPI Fox_variant_get_validated_keyed_setter(Fox_variant_type p_type);
Fox_validated_keyed_getter GDAPI Fox_variant_get_validated_keyed_getter(Fox_variant_type p_type);
Fox_validated_keyed_checker GDAPI Fox_variant_get_validated_keyed_checker(Fox_variant_type p_type);
Fox_ptr_keyed_setter GDAPI Fox_variant_get_ptr_keyed_setter(Fox_variant_type p_type);
Fox_ptr_keyed_getter GDAPI Fox_variant_get_ptr_keyed_getter(Fox_variant_type p_type);
Fox_ptr_keyed_checker GDAPI Fox_variant_get_ptr_keyed_checker(Fox_variant_type p_type);

/// Constants.
int GDAPI Fox_variant_get_constants_count(Fox_variant_type p_type);
void GDAPI Fox_variant_get_constants_list(Fox_variant_type p_type, Fox_string_name *r_list);
bool GDAPI Fox_variant_has_constant(Fox_variant_type p_type, const Fox_string_name *p_constant);
bool GDAPI Fox_variant_has_constant_with_cstring(Fox_variant_type p_type, const char *p_constant);
Fox_variant GDAPI Fox_variant_get_constant_value(Fox_variant_type p_type, const Fox_string_name *p_constant);
Fox_variant GDAPI Fox_variant_get_constant_value_with_cstring(Fox_variant_type p_type, const char *p_constant);

/// Utilities.
bool GDAPI Fox_variant_has_utility_function(const Fox_string_name *p_function);
bool GDAPI Fox_variant_has_utility_function_with_cstring(const char *p_function);
void GDAPI Fox_variant_call_utility_function(const Fox_string_name *p_function, Fox_variant *r_ret, const Fox_variant **p_args, int p_argument_count, Fox_variant_call_error *r_error);
void GDAPI Fox_variant_call_utility_function_with_cstring(const char *p_function, Fox_variant *r_ret, const Fox_variant **p_args, int p_argument_count, Fox_variant_call_error *r_error);
Fox_ptr_utility_function GDAPI Fox_variant_get_ptr_utility_function(const Fox_string_name *p_function);
Fox_ptr_utility_function GDAPI Fox_variant_get_ptr_utility_function_with_cstring(const char *p_function);
Fox_validated_utility_function GDAPI Fox_variant_get_validated_utility_function(const Fox_string_name *p_function);
Fox_validated_utility_function GDAPI Fox_variant_get_validated_utility_function_with_cstring(const char *p_function);
Fox_variant_utility_function_type GDAPI Fox_variant_get_utility_function_type(const Fox_string_name *p_function);
Fox_variant_utility_function_type GDAPI Fox_variant_get_utility_function_type_with_cstring(const char *p_function);
int GDAPI Fox_variant_get_utility_function_argument_count(const Fox_string_name *p_function);
int GDAPI Fox_variant_get_utility_function_argument_count_with_cstring(const char *p_function);
Fox_variant_type GDAPI Fox_variant_get_utility_function_argument_type(const Fox_string_name *p_function, int p_argument);
Fox_variant_type GDAPI Fox_variant_get_utility_function_argument_type_with_cstring(const char *p_function, int p_argument);
Fox_string GDAPI Fox_variant_get_utility_function_argument_name(const Fox_string_name *p_function, int p_argument);
Fox_string GDAPI Fox_variant_get_utility_function_argument_name_with_cstring(const char *p_function, int p_argument);
bool GDAPI Fox_variant_has_utility_function_return_value(const Fox_string_name *p_function);
bool GDAPI Fox_variant_has_utility_function_return_value_with_cstring(const char *p_function);
Fox_variant_type GDAPI Fox_variant_get_utility_function_return_type(const Fox_string_name *p_function);
Fox_variant_type GDAPI Fox_variant_get_utility_function_return_type_with_cstring(const char *p_function);
bool GDAPI Fox_variant_is_utility_function_vararg(const Fox_string_name *p_function);
bool GDAPI Fox_variant_is_utility_function_vararg_with_cstring(const char *p_function);
int GDAPI Fox_variant_get_utility_function_count();
void GDAPI Fox_variant_get_utility_function_list(Fox_string_name *r_functions);

// Introspection.

Fox_variant_type GDAPI Fox_variant_get_type(const Fox_variant *p_self);
bool GDAPI Fox_variant_has_method(const Fox_variant *p_self, const Fox_string_name *p_method);
bool GDAPI Fox_variant_has_member(Fox_variant_type p_type, const Fox_string_name *p_member);
bool GDAPI Fox_variant_has_key(const Fox_variant *p_self, const Fox_variant *p_key, bool *r_valid);

Fox_string GDAPI Fox_variant_get_type_name(Fox_variant_type p_type);
bool GDAPI Fox_variant_can_convert(Fox_variant_type p_from, Fox_variant_type p_to);
bool GDAPI Fox_variant_can_convert_strict(Fox_variant_type p_from, Fox_variant_type p_to);

#ifdef __cplusplus
}
#endif

#endif
