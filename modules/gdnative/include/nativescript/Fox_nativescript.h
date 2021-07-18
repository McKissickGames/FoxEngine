/*************************************************************************/
/*  Fox_nativescript.h                                                 */
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

#ifndef Fox_NATIVESCRIPT_H
#define Fox_NATIVESCRIPT_H

#include <gdnative/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	Fox_METHOD_RPC_MODE_DISABLED,
	Fox_METHOD_RPC_MODE_REMOTE,
	Fox_METHOD_RPC_MODE_MASTER,
	Fox_METHOD_RPC_MODE_PUPPET,
	Fox_METHOD_RPC_MODE_REMOTESYNC,
	Fox_METHOD_RPC_MODE_MASTERSYNC,
	Fox_METHOD_RPC_MODE_PUPPETSYNC,
} Fox_nativescript_method_rpc_mode;

typedef enum {
	Fox_PROPERTY_HINT_NONE, ///< no hint provided.
	Fox_PROPERTY_HINT_RANGE, ///< hint_text = "min,max,step,slider; //slider is optional"
	Fox_PROPERTY_HINT_EXP_RANGE, ///< hint_text = "min,max,step", exponential edit
	Fox_PROPERTY_HINT_ENUM, ///< hint_text= "val1,val2,val3,etc"
	Fox_PROPERTY_HINT_EXP_EASING, /// exponential easing function (Math::ease)
	Fox_PROPERTY_HINT_LENGTH, ///< hint_text= "length" (as integer)
	Fox_PROPERTY_HINT_KEY_ACCEL, ///< hint_text= "length" (as integer)
	Fox_PROPERTY_HINT_FLAGS, ///< hint_text= "flag1,flag2,etc" (as bit flags)
	Fox_PROPERTY_HINT_LAYERS_2D_RENDER,
	Fox_PROPERTY_HINT_LAYERS_2D_PHYSICS,
	Fox_PROPERTY_HINT_LAYERS_2D_NAVIGATION,
	Fox_PROPERTY_HINT_LAYERS_3D_RENDER,
	Fox_PROPERTY_HINT_LAYERS_3D_PHYSICS,
	Fox_PROPERTY_HINT_LAYERS_3D_NAVIGATION,
	Fox_PROPERTY_HINT_FILE, ///< a file path must be passed, hint_text (optionally) is a filter "*.png,*.wav,*.doc,"
	Fox_PROPERTY_HINT_DIR, ///< a directory path must be passed
	Fox_PROPERTY_HINT_GLOBAL_FILE, ///< a file path must be passed, hint_text (optionally) is a filter "*.png,*.wav,*.doc,"
	Fox_PROPERTY_HINT_GLOBAL_DIR, ///< a directory path must be passed
	Fox_PROPERTY_HINT_RESOURCE_TYPE, ///< a resource object type
	Fox_PROPERTY_HINT_MULTILINE_TEXT, ///< used for string properties that can contain multiple lines
	Fox_PROPERTY_HINT_PLACEHOLDER_TEXT, ///< used to set a placeholder text for string properties
	Fox_PROPERTY_HINT_COLOR_NO_ALPHA, ///< used for ignoring alpha component when editing a color
	Fox_PROPERTY_HINT_IMAGE_COMPRESS_LOSSY,
	Fox_PROPERTY_HINT_IMAGE_COMPRESS_LOSSLESS,
	Fox_PROPERTY_HINT_OBJECT_ID,
	Fox_PROPERTY_HINT_TYPE_STRING, ///< a type string, the hint is the base type to choose
	Fox_PROPERTY_HINT_NODE_PATH_TO_EDITED_NODE, ///< so something else can provide this (used in scripts)
	Fox_PROPERTY_HINT_METHOD_OF_VARIANT_TYPE, ///< a method of a type
	Fox_PROPERTY_HINT_METHOD_OF_BASE_TYPE, ///< a method of a base type
	Fox_PROPERTY_HINT_METHOD_OF_INSTANCE, ///< a method of an instance
	Fox_PROPERTY_HINT_METHOD_OF_SCRIPT, ///< a method of a script & base
	Fox_PROPERTY_HINT_PROPERTY_OF_VARIANT_TYPE, ///< a property of a type
	Fox_PROPERTY_HINT_PROPERTY_OF_BASE_TYPE, ///< a property of a base type
	Fox_PROPERTY_HINT_PROPERTY_OF_INSTANCE, ///< a property of an instance
	Fox_PROPERTY_HINT_PROPERTY_OF_SCRIPT, ///< a property of a script & base
	Fox_PROPERTY_HINT_MAX,
} Fox_nativescript_property_hint;

typedef enum {
	Fox_PROPERTY_USAGE_STORAGE = 1,
	Fox_PROPERTY_USAGE_EDITOR = 2,
	Fox_PROPERTY_USAGE_NETWORK = 4,
	Fox_PROPERTY_USAGE_EDITOR_HELPER = 8,
	Fox_PROPERTY_USAGE_CHECKABLE = 16, //used for editing global variables
	Fox_PROPERTY_USAGE_CHECKED = 32, //used for editing global variables
	Fox_PROPERTY_USAGE_INTERNATIONALIZED = 64, //hint for internationalized strings
	Fox_PROPERTY_USAGE_GROUP = 128, //used for grouping props in the editor
	Fox_PROPERTY_USAGE_CATEGORY = 256,
	Fox_PROPERTY_USAGE_SUBGROUP = 512,
	Fox_PROPERTY_USAGE_NO_INSTANCE_STATE = 2048,
	Fox_PROPERTY_USAGE_RESTART_IF_CHANGED = 4096,
	Fox_PROPERTY_USAGE_SCRIPT_VARIABLE = 8192,
	Fox_PROPERTY_USAGE_STORE_IF_NULL = 16384,
	Fox_PROPERTY_USAGE_ANIMATE_AS_TRIGGER = 32768,
	Fox_PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED = 65536,

	Fox_PROPERTY_USAGE_DEFAULT = Fox_PROPERTY_USAGE_STORAGE | Fox_PROPERTY_USAGE_EDITOR | Fox_PROPERTY_USAGE_NETWORK,
	Fox_PROPERTY_USAGE_DEFAULT_INTL = Fox_PROPERTY_USAGE_STORAGE | Fox_PROPERTY_USAGE_EDITOR | Fox_PROPERTY_USAGE_NETWORK | Fox_PROPERTY_USAGE_INTERNATIONALIZED,
	Fox_PROPERTY_USAGE_NOEDITOR = Fox_PROPERTY_USAGE_STORAGE | Fox_PROPERTY_USAGE_NETWORK,
} Fox_nativescript_property_usage_flags;

typedef struct {
	Fox_nativescript_method_rpc_mode rset_type;

	Fox_int type;
	Fox_nativescript_property_hint hint;
	Fox_string hint_string;
	Fox_nativescript_property_usage_flags usage;
	Fox_variant default_value;
} Fox_nativescript_property_attributes;

typedef struct {
	// instance pointer, method_data - return user data
	GDCALLINGCONV void *(*create_func)(Fox_object *, void *);
	void *method_data;
	GDCALLINGCONV void (*free_func)(void *);
} Fox_nativescript_instance_create_func;

typedef struct {
	// instance pointer, method data, user data
	GDCALLINGCONV void (*destroy_func)(Fox_object *, void *, void *);
	void *method_data;
	GDCALLINGCONV void (*free_func)(void *);
} Fox_nativescript_instance_destroy_func;

void GDAPI Fox_nativescript_register_class(void *p_gdnative_handle, const char *p_name, const char *p_base, Fox_nativescript_instance_create_func p_create_func, Fox_nativescript_instance_destroy_func p_destroy_func);

void GDAPI Fox_nativescript_register_tool_class(void *p_gdnative_handle, const char *p_name, const char *p_base, Fox_nativescript_instance_create_func p_create_func, Fox_nativescript_instance_destroy_func p_destroy_func);

typedef struct {
	Fox_nativescript_method_rpc_mode rpc_type;
} Fox_nativescript_method_attributes;

typedef struct {
	Fox_string name;

	Fox_variant_type type;
	Fox_nativescript_property_hint hint;
	Fox_string hint_string;
} Fox_nativescript_method_argument;

typedef struct {
	// instance pointer, method data, user data, num args, args - return result as varaint
	GDCALLINGCONV Fox_variant (*method)(Fox_object *, void *, void *, int, Fox_variant **);
	void *method_data;
	GDCALLINGCONV void (*free_func)(void *);
} Fox_nativescript_instance_method;

void GDAPI Fox_nativescript_register_method(void *p_gdnative_handle, const char *p_name, const char *p_function_name, Fox_nativescript_method_attributes p_attr, Fox_nativescript_instance_method p_method);
void GDAPI Fox_nativescript_set_method_argument_information(void *p_gdnative_handle, const char *p_name, const char *p_function_name, int p_num_args, const Fox_nativescript_method_argument *p_args);

typedef struct {
	// instance pointer, method data, user data, value
	GDCALLINGCONV void (*set_func)(Fox_object *, void *, void *, Fox_variant *);
	void *method_data;
	GDCALLINGCONV void (*free_func)(void *);
} Fox_nativescript_property_set_func;

typedef struct {
	// instance pointer, method data, user data, value
	GDCALLINGCONV Fox_variant (*get_func)(Fox_object *, void *, void *);
	void *method_data;
	GDCALLINGCONV void (*free_func)(void *);
} Fox_nativescript_property_get_func;

void GDAPI Fox_nativescript_register_property(void *p_gdnative_handle, const char *p_name, const char *p_path, Fox_nativescript_property_attributes *p_attr, Fox_nativescript_property_set_func p_set_func, Fox_nativescript_property_get_func p_get_func);

typedef struct {
	Fox_string name;
	Fox_int type;
	Fox_nativescript_property_hint hint;
	Fox_string hint_string;
	Fox_nativescript_property_usage_flags usage;
	Fox_variant default_value;
} Fox_nativescript_signal_argument;

typedef struct {
	Fox_string name;
	int num_args;
	Fox_nativescript_signal_argument *args;
	int num_default_args;
	Fox_variant *default_args;
} Fox_nativescript_signal;

void GDAPI Fox_nativescript_register_signal(void *p_gdnative_handle, const char *p_name, const Fox_nativescript_signal *p_signal);

void GDAPI *Fox_nativescript_get_userdata(Fox_object *p_instance);

// documentation

void GDAPI Fox_nativescript_set_class_documentation(void *p_gdnative_handle, const char *p_name, Fox_string p_documentation);
void GDAPI Fox_nativescript_set_method_documentation(void *p_gdnative_handle, const char *p_name, const char *p_function_name, Fox_string p_documentation);
void GDAPI Fox_nativescript_set_property_documentation(void *p_gdnative_handle, const char *p_name, const char *p_path, Fox_string p_documentation);
void GDAPI Fox_nativescript_set_signal_documentation(void *p_gdnative_handle, const char *p_name, const char *p_signal_name, Fox_string p_documentation);

// type tag API

void GDAPI Fox_nativescript_set_global_type_tag(int p_idx, const char *p_name, const void *p_type_tag);
const void GDAPI *Fox_nativescript_get_global_type_tag(int p_idx, const char *p_name);

void GDAPI Fox_nativescript_set_type_tag(void *p_gdnative_handle, const char *p_name, const void *p_type_tag);
const void GDAPI *Fox_nativescript_get_type_tag(const Fox_object *p_object);

// instance binding API

typedef struct {
	GDCALLINGCONV void *(*alloc_instance_binding_data)(void *, const void *, Fox_object *);
	GDCALLINGCONV void (*free_instance_binding_data)(void *, void *);
	GDCALLINGCONV void (*refcount_incremented_instance_binding)(void *, Fox_object *);
	GDCALLINGCONV bool (*refcount_decremented_instance_binding)(void *, Fox_object *);
	void *data;
	GDCALLINGCONV void (*free_func)(void *);
} Fox_nativescript_instance_binding_functions;

int GDAPI Fox_nativescript_register_instance_binding_data_functions(Fox_nativescript_instance_binding_functions p_binding_functions);
void GDAPI Fox_nativescript_unregister_instance_binding_data_functions(int p_idx);

void GDAPI *Fox_nativescript_get_instance_binding_data(int p_idx, Fox_object *p_object);

void GDAPI Fox_nativescript_profiling_add_data(const char *p_signature, uint64_t p_time);

#ifdef __cplusplus
}
#endif

#endif
