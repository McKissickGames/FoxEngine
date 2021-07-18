/*************************************************************************/
/*  Fox_pluginscript.h                                                 */
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

#ifndef Fox_PLUGINSCRIPT_H
#define Fox_PLUGINSCRIPT_H

#include <gdnative/gdnative.h>
#include <nativescript/Fox_nativescript.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void Fox_pluginscript_instance_data;
typedef void Fox_pluginscript_script_data;
typedef void Fox_pluginscript_language_data;

// --- Instance ---

typedef struct {
	Fox_pluginscript_instance_data *(*init)(Fox_pluginscript_script_data *p_data, Fox_object *p_owner);
	void (*finish)(Fox_pluginscript_instance_data *p_data);

	Fox_bool (*set_prop)(Fox_pluginscript_instance_data *p_data, const Fox_string_name *p_name, const Fox_variant *p_value);
	Fox_bool (*get_prop)(Fox_pluginscript_instance_data *p_data, const Fox_string_name *p_name, Fox_variant *r_ret);

	Fox_variant (*call_method)(Fox_pluginscript_instance_data *p_data,
			const Fox_string_name *p_method, const Fox_variant **p_args,
			int p_argcount, Fox_variant_call_error *r_error);

	void (*notification)(Fox_pluginscript_instance_data *p_data, int p_notification);
	Fox_string (*to_string)(Fox_pluginscript_instance_data *p_data, Fox_bool *r_valid);

	//this is used by script languages that keep a reference counter of their own
	//you can make make Ref<> not die when it reaches zero, so deleting the reference
	//depends entirely from the script.
	// Note: You can set those function pointer to nullptr if not needed.
	void (*refcount_incremented)(Fox_pluginscript_instance_data *p_data);
	bool (*refcount_decremented)(Fox_pluginscript_instance_data *p_data); // return true if it can die
} Fox_pluginscript_instance_desc;

// --- Script ---

typedef struct {
	Fox_pluginscript_script_data *data;
	Fox_string_name name;
	Fox_bool is_tool;
	Fox_string_name base;
	Fox_string icon_path;

	// Member lines format: {<string>: <int>}
	Fox_dictionary member_lines;
	// Method info dictionary format
	// {
	//  name: <string>
	//  args: [<dict:property>]
	//  default_args: [<variant>]
	//  return: <dict:property>
	//  flags: <int>
	//  rpc_mode: <int:Fox_method_rpc_mode>
	// }
	Fox_array methods;
	// Same format than for methods
	Fox_array signals;
	// Property info dictionary format
	// {
	//  name: <string>
	//  type: <int:Fox_variant_type>
	//  hint: <int:Fox_property_hint>
	//  hint_string: <string>
	//  usage: <int:Fox_property_usage_flags>
	//  default_value: <variant>
	//  rset_mode: <int:Fox_method_rpc_mode>
	// }
	Fox_array properties;
} Fox_pluginscript_script_manifest;

typedef struct {
	Fox_pluginscript_script_manifest (*init)(Fox_pluginscript_language_data *p_data, const Fox_string *p_path, const Fox_string *p_source, Fox_error *r_error);
	void (*finish)(Fox_pluginscript_script_data *p_data);
	Fox_pluginscript_instance_desc instance_desc;
} Fox_pluginscript_script_desc;

// --- Language ---

typedef struct {
	Fox_string_name signature;
	Fox_int call_count;
	Fox_int total_time; // In microseconds
	Fox_int self_time; // In microseconds
} Fox_pluginscript_profiling_data;

typedef struct {
	const char *name;
	const char *type;
	const char *extension;
	const char **recognized_extensions; // nullptr terminated array
	Fox_pluginscript_language_data *(*init)();
	void (*finish)(Fox_pluginscript_language_data *p_data);
	const char **reserved_words; // nullptr terminated array
	const char **comment_delimiters; // nullptr terminated array
	const char **string_delimiters; // nullptr terminated array
	Fox_bool has_named_classes;
	Fox_bool supports_builtin_mode;
	Fox_bool can_inherit_from_file;

	Fox_string (*get_template_source_code)(Fox_pluginscript_language_data *p_data, const Fox_string *p_class_name, const Fox_string *p_base_class_name);
	Fox_bool (*validate)(Fox_pluginscript_language_data *p_data, const Fox_string *p_script, const Fox_string *p_path, Fox_packed_string_array *r_functions, Fox_array *r_errors); // errors = Array of Dictionary with "line", "column", "message" keys
	int (*find_function)(Fox_pluginscript_language_data *p_data, const Fox_string *p_function, const Fox_string *p_code); // Can be nullptr
	Fox_string (*make_function)(Fox_pluginscript_language_data *p_data, const Fox_string *p_class, const Fox_string *p_name, const Fox_packed_string_array *p_args);
	Fox_error (*complete_code)(Fox_pluginscript_language_data *p_data, const Fox_string *p_code, const Fox_string *p_path, Fox_object *p_owner, Fox_array *r_options, Fox_bool *r_force, Fox_string *r_call_hint);
	void (*auto_indent_code)(Fox_pluginscript_language_data *p_data, Fox_string *p_code, int p_from_line, int p_to_line);

	void (*add_global_constant)(Fox_pluginscript_language_data *p_data, const Fox_string_name *p_variable, const Fox_variant *p_value);
	Fox_string (*debug_get_error)(Fox_pluginscript_language_data *p_data);
	int (*debug_get_stack_level_count)(Fox_pluginscript_language_data *p_data);
	int (*debug_get_stack_level_line)(Fox_pluginscript_language_data *p_data, int p_level);
	Fox_string (*debug_get_stack_level_function)(Fox_pluginscript_language_data *p_data, int p_level);
	Fox_string (*debug_get_stack_level_source)(Fox_pluginscript_language_data *p_data, int p_level);
	void (*debug_get_stack_level_locals)(Fox_pluginscript_language_data *p_data, int p_level, Fox_packed_string_array *p_locals, Fox_array *p_values, int p_max_subitems, int p_max_depth);
	void (*debug_get_stack_level_members)(Fox_pluginscript_language_data *p_data, int p_level, Fox_packed_string_array *p_members, Fox_array *p_values, int p_max_subitems, int p_max_depth);
	void (*debug_get_globals)(Fox_pluginscript_language_data *p_data, Fox_packed_string_array *p_locals, Fox_array *p_values, int p_max_subitems, int p_max_depth);
	Fox_string (*debug_parse_stack_level_expression)(Fox_pluginscript_language_data *p_data, int p_level, const Fox_string *p_expression, int p_max_subitems, int p_max_depth);

	// TODO: could this stuff be moved to the Fox_pluginscript_language_desc ?
	void (*get_public_functions)(Fox_pluginscript_language_data *p_data, Fox_array *r_functions);
	void (*get_public_constants)(Fox_pluginscript_language_data *p_data, Fox_dictionary *r_constants);

	void (*profiling_start)(Fox_pluginscript_language_data *p_data);
	void (*profiling_stop)(Fox_pluginscript_language_data *p_data);
	int (*profiling_get_accumulated_data)(Fox_pluginscript_language_data *p_data, Fox_pluginscript_profiling_data *r_info, int p_info_max);
	int (*profiling_get_frame_data)(Fox_pluginscript_language_data *p_data, Fox_pluginscript_profiling_data *r_info, int p_info_max);
	void (*profiling_frame)(Fox_pluginscript_language_data *p_data);

	Fox_pluginscript_script_desc script_desc;
} Fox_pluginscript_language_desc;

void GDAPI Fox_pluginscript_register_language(const Fox_pluginscript_language_desc *language_desc);

#ifdef __cplusplus
}
#endif

#endif // Fox_PLUGINSCRIPT_H
