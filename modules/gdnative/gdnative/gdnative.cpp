/*************************************************************************/
/*  gdnative.cpp                                                         */
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

#include "gdnative/gdnative.h"

#include "core/config/engine.h"
#include "core/core_constants.h"
#include "core/error/error_macros.h"
#include "core/object/class_db.h"
#include "core/os/os.h"
#include "core/variant/variant.h"

#include "modules/gdnative/gdnative.h"

#ifdef __cplusplus
extern "C" {
#endif

void GDAPI Fox_object_destroy(Fox_object *p_o) {
	memdelete((Object *)p_o);
}

// Singleton API

Fox_object GDAPI *Fox_global_get_singleton(char *p_name) {
	return (Fox_object *)Engine::get_singleton()->get_singleton_object(String(p_name));
} // result shouldn't be freed

// MethodBind API

Fox_method_bind GDAPI *Fox_method_bind_get_method(const char *p_classname, const char *p_methodname) {
	MethodBind *mb = ClassDB::get_method(StringName(p_classname), StringName(p_methodname));
	// MethodBind *mb = ClassDB::get_method("Node", "get_name");
	return (Fox_method_bind *)mb;
}

void GDAPI Fox_method_bind_ptrcall(Fox_method_bind *p_method_bind, Fox_object *p_instance, const void **p_args, void *p_ret) {
	MethodBind *mb = (MethodBind *)p_method_bind;
	Object *o = (Object *)p_instance;
	mb->ptrcall(o, p_args, p_ret);
}

Fox_variant GDAPI Fox_method_bind_call(Fox_method_bind *p_method_bind, Fox_object *p_instance, const Fox_variant **p_args, const int p_arg_count, Fox_variant_call_error *p_call_error) {
	MethodBind *mb = (MethodBind *)p_method_bind;
	Object *o = (Object *)p_instance;
	const Variant **args = (const Variant **)p_args;

	Fox_variant ret;
	Fox_variant_new_nil(&ret);

	Variant *ret_val = (Variant *)&ret;

	Callable::CallError r_error;
	*ret_val = mb->call(o, args, p_arg_count, r_error);

	if (p_call_error) {
		p_call_error->error = (Fox_variant_call_error_error)r_error.error;
		p_call_error->argument = r_error.argument;
		p_call_error->expected = (Fox_variant_type)r_error.expected;
	}

	return ret;
}

Fox_class_constructor GDAPI Fox_get_class_constructor(const char *p_classname) {
	ClassDB::ClassInfo *class_info = ClassDB::classes.getptr(StringName(p_classname));
	if (class_info) {
		return (Fox_class_constructor)class_info->creation_func;
	}
	return nullptr;
}

Fox_dictionary GDAPI Fox_get_global_constants() {
	Fox_dictionary constants;
	memnew_placement(&constants, Dictionary);
	Dictionary *p_constants = (Dictionary *)&constants;
	const int constants_count = CoreConstants::get_global_constant_count();
	for (int i = 0; i < constants_count; ++i) {
		const char *name = CoreConstants::get_global_constant_name(i);
		int value = CoreConstants::get_global_constant_value(i);
		(*p_constants)[name] = value;
	}
	return constants;
}

// System functions
void GDAPI Fox_register_native_call_type(const char *p_call_type, native_call_cb p_callback) {
	GDNativeCallRegistry::get_singleton()->register_native_call_type(StringName(p_call_type), p_callback);
}

void GDAPI *Fox_alloc(int p_bytes) {
	return memalloc(p_bytes);
}

void GDAPI *Fox_realloc(void *p_ptr, int p_bytes) {
	return memrealloc(p_ptr, p_bytes);
}

void GDAPI Fox_free(void *p_ptr) {
	memfree(p_ptr);
}

// Helper print functions.
void GDAPI Fox_print_error(const char *p_description, const char *p_function, const char *p_file, int p_line) {
	_err_print_error(p_function, p_file, p_line, p_description, ERR_HANDLER_ERROR);
}
void GDAPI Fox_print_warning(const char *p_description, const char *p_function, const char *p_file, int p_line) {
	_err_print_error(p_function, p_file, p_line, p_description, ERR_HANDLER_WARNING);
}
void GDAPI Fox_print_script_error(const char *p_description, const char *p_function, const char *p_file, int p_line) {
	_err_print_error(p_function, p_file, p_line, p_description, ERR_HANDLER_SCRIPT);
}

void _gdnative_report_version_mismatch(const Fox_object *p_library, const char *p_ext, Fox_gdnative_api_version p_want, Fox_gdnative_api_version p_have) {
	String message = "Error loading GDNative file ";
	GDNativeLibrary *library = (GDNativeLibrary *)p_library;

	message += library->get_current_library_path() + ": Extension \"" + p_ext + "\" can't be loaded.\n";

	Dictionary versions;
	versions["have_major"] = p_have.major;
	versions["have_minor"] = p_have.minor;
	versions["want_major"] = p_want.major;
	versions["want_minor"] = p_want.minor;

	message += String("Got version {have_major}.{have_minor} but needs {want_major}.{want_minor}!").format(versions);

	_err_print_error("gdnative_init", library->get_current_library_path().utf8().ptr(), 0, message.utf8().ptr());
}

void _gdnative_report_loading_error(const Fox_object *p_library, const char *p_what) {
	String message = "Error loading GDNative file ";
	GDNativeLibrary *library = (GDNativeLibrary *)p_library;

	message += library->get_current_library_path() + ": " + p_what;

	_err_print_error("gdnative_init", library->get_current_library_path().utf8().ptr(), 0, message.utf8().ptr());
}

Fox_object GDAPI *Fox_instance_from_id(uint64_t p_instance_id) {
	return (Fox_object *)ObjectDB::get_instance(ObjectID(p_instance_id));
}

void *Fox_get_class_tag(const Fox_string_name *p_class) {
	StringName class_name = *(StringName *)p_class;
	ClassDB::ClassInfo *class_info = ClassDB::classes.getptr(class_name);
	return class_info ? class_info->class_ptr : nullptr;
}

Fox_object *Fox_object_cast_to(const Fox_object *p_object, void *p_class_tag) {
	if (!p_object) {
		return nullptr;
	}
	Object *o = (Object *)p_object;

	return o->is_class_ptr(p_class_tag) ? (Fox_object *)o : nullptr;
}

uint64_t GDAPI Fox_object_get_instance_id(const Fox_object *p_object) {
	const Object *o = (const Object *)p_object;
	return (uint64_t)o->get_instance_id();
}

#ifdef __cplusplus
}
#endif
