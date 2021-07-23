/*************************************************************************/
/*  gd_glue.cpp                                                          */
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

#ifdef MONO_GLUE_ENABLED

#include "core/io/marshalls.h"
#include "core/os/os.h"
#include "core/string/ustring.h"
#include "core/variant/array.h"
#include "core/variant/variant.h"
#include "core/variant/variant_parser.h"

#include "../mono_gd/gd_mono_cache.h"
#include "../mono_gd/gd_mono_marshal.h"
#include "../mono_gd/gd_mono_utils.h"

MonoObject *Fox_icall_GD_bytes2var(MonoArray *p_bytes, MonoBoolean p_allow_objects) {
	Variant ret;
	PackedByteArray varr = GDMonoMarshal::mono_array_to_PackedByteArray(p_bytes);
	Error err = decode_variant(ret, varr.ptr(), varr.size(), nullptr, p_allow_objects);
	if (err != OK) {
		ret = RTR("Not enough bytes for decoding bytes, or invalid format.");
	}
	return GDMonoMarshal::variant_to_mono_object(ret);
}

MonoObject *Fox_icall_GD_convert(MonoObject *p_what, int32_t p_type) {
	Variant what = GDMonoMarshal::mono_object_to_variant(p_what);
	const Variant *args[1] = { &what };
	Callable::CallError ce;
	Variant ret;
	Variant::construct(Variant::Type(p_type), ret, args, 1, ce);
	ERR_FAIL_COND_V(ce.error != Callable::CallError::CALL_OK, nullptr);
	return GDMonoMarshal::variant_to_mono_object(ret);
}

int Fox_icall_GD_hash(MonoObject *p_var) {
	return GDMonoMarshal::mono_object_to_variant(p_var).hash();
}

MonoObject *Fox_icall_GD_instance_from_id(uint64_t p_instance_id) {
	return GDMonoUtils::unmanaged_get_managed(ObjectDB::get_instance(ObjectID(p_instance_id)));
}

void Fox_icall_GD_print(MonoArray *p_what) {
	String str;
	int length = mono_array_length(p_what);

	for (int i = 0; i < length; i++) {
		MonoObject *elem = mono_array_get(p_what, MonoObject *, i);

		MonoException *exc = nullptr;
		String elem_str = GDMonoMarshal::mono_object_to_variant_string(elem, &exc);

		if (exc) {
			GDMonoUtils::set_pending_exception(exc);
			return;
		}

		str += elem_str;
	}

	print_line(str);
}

void Fox_icall_GD_printerr(MonoArray *p_what) {
	String str;
	int length = mono_array_length(p_what);

	for (int i = 0; i < length; i++) {
		MonoObject *elem = mono_array_get(p_what, MonoObject *, i);

		MonoException *exc = nullptr;
		String elem_str = GDMonoMarshal::mono_object_to_variant_string(elem, &exc);

		if (exc) {
			GDMonoUtils::set_pending_exception(exc);
			return;
		}

		str += elem_str;
	}

	print_error(str);
}

void Fox_icall_GD_printraw(MonoArray *p_what) {
	String str;
	int length = mono_array_length(p_what);

	for (int i = 0; i < length; i++) {
		MonoObject *elem = mono_array_get(p_what, MonoObject *, i);

		MonoException *exc = nullptr;
		String elem_str = GDMonoMarshal::mono_object_to_variant_string(elem, &exc);

		if (exc) {
			GDMonoUtils::set_pending_exception(exc);
			return;
		}

		str += elem_str;
	}

	OS::get_singleton()->print("%s", str.utf8().get_data());
}

void Fox_icall_GD_prints(MonoArray *p_what) {
	String str;
	int length = mono_array_length(p_what);

	for (int i = 0; i < length; i++) {
		MonoObject *elem = mono_array_get(p_what, MonoObject *, i);

		MonoException *exc = nullptr;
		String elem_str = GDMonoMarshal::mono_object_to_variant_string(elem, &exc);

		if (exc) {
			GDMonoUtils::set_pending_exception(exc);
			return;
		}

		if (i) {
			str += " ";
		}

		str += elem_str;
	}

	print_line(str);
}

void Fox_icall_GD_printt(MonoArray *p_what) {
	String str;
	int length = mono_array_length(p_what);

	for (int i = 0; i < length; i++) {
		MonoObject *elem = mono_array_get(p_what, MonoObject *, i);

		MonoException *exc = nullptr;
		String elem_str = GDMonoMarshal::mono_object_to_variant_string(elem, &exc);

		if (exc) {
			GDMonoUtils::set_pending_exception(exc);
			return;
		}

		if (i) {
			str += "\t";
		}

		str += elem_str;
	}

	print_line(str);
}

float Fox_icall_GD_randf() {
	return Math::randf();
}

uint32_t Fox_icall_GD_randi() {
	return Math::rand();
}

void Fox_icall_GD_randomize() {
	Math::randomize();
}

double Fox_icall_GD_randf_range(double from, double to) {
	return Math::random(from, to);
}

int32_t Fox_icall_GD_randi_range(int32_t from, int32_t to) {
	return Math::random(from, to);
}

uint32_t Fox_icall_GD_rand_seed(uint64_t seed, uint64_t *newSeed) {
	uint32_t ret = Math::rand_from_seed(&seed);
	*newSeed = seed;
	return ret;
}

void Fox_icall_GD_seed(uint64_t p_seed) {
	Math::seed(p_seed);
}

MonoString *Fox_icall_GD_str(MonoArray *p_what) {
	String str;
	Array what = GDMonoMarshal::mono_array_to_Array(p_what);

	for (int i = 0; i < what.size(); i++) {
		String os = what[i].operator String();

		if (i == 0) {
			str = os;
		} else {
			str += os;
		}
	}

	return GDMonoMarshal::mono_string_from_Fox(str);
}

MonoObject *Fox_icall_GD_str2var(MonoString *p_str) {
	Variant ret;

	VariantParser::StreamString ss;
	ss.s = GDMonoMarshal::mono_string_to_Fox(p_str);

	String errs;
	int line;
	Error err = VariantParser::parse(&ss, ret, errs, line);
	if (err != OK) {
		String err_str = "Parse error at line " + itos(line) + ": " + errs + ".";
		ERR_PRINT(err_str);
		ret = err_str;
	}

	return GDMonoMarshal::variant_to_mono_object(ret);
}

MonoBoolean Fox_icall_GD_type_exists(StringName *p_type) {
	StringName type = p_type ? *p_type : StringName();
	return ClassDB::class_exists(type);
}

void Fox_icall_GD_pusherror(MonoString *p_str) {
	ERR_PRINT(GDMonoMarshal::mono_string_to_Fox(p_str));
}

void Fox_icall_GD_pushwarning(MonoString *p_str) {
	WARN_PRINT(GDMonoMarshal::mono_string_to_Fox(p_str));
}

MonoArray *Fox_icall_GD_var2bytes(MonoObject *p_var, MonoBoolean p_full_objects) {
	Variant var = GDMonoMarshal::mono_object_to_variant(p_var);

	PackedByteArray barr;
	int len;
	Error err = encode_variant(var, nullptr, len, p_full_objects);
	ERR_FAIL_COND_V_MSG(err != OK, nullptr, "Unexpected error encoding variable to bytes, likely unserializable type found (Object or RID).");

	barr.resize(len);
	encode_variant(var, barr.ptrw(), len, p_full_objects);

	return GDMonoMarshal::PackedByteArray_to_mono_array(barr);
}

MonoString *Fox_icall_GD_var2str(MonoObject *p_var) {
	String vars;
	VariantWriter::write_to_string(GDMonoMarshal::mono_object_to_variant(p_var), vars);
	return GDMonoMarshal::mono_string_from_Fox(vars);
}

uint32_t Fox_icall_TypeToVariantType(MonoReflectionType *p_refl_type) {
	return (uint32_t)GDMonoMarshal::managed_to_variant_type(ManagedType::from_reftype(p_refl_type));
}

MonoObject *Fox_icall_DefaultFoxTaskScheduler() {
	return GDMonoCache::cached_data.task_scheduler_handle->get_target();
}

void Fox_register_gd_icalls() {
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_GD_bytes2var", Fox_icall_GD_bytes2var);
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_GD_convert", Fox_icall_GD_convert);
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_GD_hash", Fox_icall_GD_hash);
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_GD_instance_from_id", Fox_icall_GD_instance_from_id);
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_GD_pusherror", Fox_icall_GD_pusherror);
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_GD_pushwarning", Fox_icall_GD_pushwarning);
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_GD_print", Fox_icall_GD_print);
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_GD_printerr", Fox_icall_GD_printerr);
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_GD_printraw", Fox_icall_GD_printraw);
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_GD_prints", Fox_icall_GD_prints);
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_GD_printt", Fox_icall_GD_printt);
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_GD_randf", Fox_icall_GD_randf);
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_GD_randi", Fox_icall_GD_randi);
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_GD_randomize", Fox_icall_GD_randomize);
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_GD_randf_range", Fox_icall_GD_randf_range);
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_GD_randi_range", Fox_icall_GD_randi_range);
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_GD_rand_seed", Fox_icall_GD_rand_seed);
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_GD_seed", Fox_icall_GD_seed);
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_GD_str", Fox_icall_GD_str);
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_GD_str2var", Fox_icall_GD_str2var);
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_GD_type_exists", Fox_icall_GD_type_exists);
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_GD_var2bytes", Fox_icall_GD_var2bytes);
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_GD_var2str", Fox_icall_GD_var2str);
	GDMonoUtils::add_internal_call("Fox.GD::Fox_icall_TypeToVariantType", Fox_icall_TypeToVariantType);

	// Dispatcher
	GDMonoUtils::add_internal_call("Fox.Dispatcher::Fox_icall_DefaultFoxTaskScheduler", Fox_icall_DefaultFoxTaskScheduler);
}

#endif // MONO_GLUE_ENABLED
