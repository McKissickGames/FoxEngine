/*************************************************************************/
/*  nodepath_glue.cpp                                                    */
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

#include "core/string/node_path.h"
#include "core/string/ustring.h"

#include "../mono_gd/gd_mono_marshal.h"

NodePath *Fox_icall_NodePath_Ctor(MonoString *p_path) {
	return memnew(NodePath(GDMonoMarshal::mono_string_to_Fox(p_path)));
}

void Fox_icall_NodePath_Dtor(NodePath *p_ptr) {
	ERR_FAIL_NULL(p_ptr);
	memdelete(p_ptr);
}

MonoString *Fox_icall_NodePath_operator_String(NodePath *p_np) {
	return GDMonoMarshal::mono_string_from_Fox(p_np->operator String());
}

MonoBoolean Fox_icall_NodePath_is_absolute(NodePath *p_ptr) {
	return (MonoBoolean)p_ptr->is_absolute();
}

int32_t Fox_icall_NodePath_get_name_count(NodePath *p_ptr) {
	return p_ptr->get_name_count();
}

MonoString *Fox_icall_NodePath_get_name(NodePath *p_ptr, uint32_t p_idx) {
	return GDMonoMarshal::mono_string_from_Fox(p_ptr->get_name(p_idx));
}

int32_t Fox_icall_NodePath_get_subname_count(NodePath *p_ptr) {
	return p_ptr->get_subname_count();
}

MonoString *Fox_icall_NodePath_get_subname(NodePath *p_ptr, uint32_t p_idx) {
	return GDMonoMarshal::mono_string_from_Fox(p_ptr->get_subname(p_idx));
}

MonoString *Fox_icall_NodePath_get_concatenated_subnames(NodePath *p_ptr) {
	return GDMonoMarshal::mono_string_from_Fox(p_ptr->get_concatenated_subnames());
}

NodePath *Fox_icall_NodePath_get_as_property_path(NodePath *p_ptr) {
	return memnew(NodePath(p_ptr->get_as_property_path()));
}

MonoBoolean Fox_icall_NodePath_is_empty(NodePath *p_ptr) {
	return (MonoBoolean)p_ptr->is_empty();
}

void Fox_register_nodepath_icalls() {
	GDMonoUtils::add_internal_call("Fox.NodePath::Fox_icall_NodePath_Ctor", Fox_icall_NodePath_Ctor);
	GDMonoUtils::add_internal_call("Fox.NodePath::Fox_icall_NodePath_Dtor", Fox_icall_NodePath_Dtor);
	GDMonoUtils::add_internal_call("Fox.NodePath::Fox_icall_NodePath_operator_String", Fox_icall_NodePath_operator_String);
	GDMonoUtils::add_internal_call("Fox.NodePath::Fox_icall_NodePath_get_as_property_path", Fox_icall_NodePath_get_as_property_path);
	GDMonoUtils::add_internal_call("Fox.NodePath::Fox_icall_NodePath_get_concatenated_subnames", Fox_icall_NodePath_get_concatenated_subnames);
	GDMonoUtils::add_internal_call("Fox.NodePath::Fox_icall_NodePath_get_name", Fox_icall_NodePath_get_name);
	GDMonoUtils::add_internal_call("Fox.NodePath::Fox_icall_NodePath_get_name_count", Fox_icall_NodePath_get_name_count);
	GDMonoUtils::add_internal_call("Fox.NodePath::Fox_icall_NodePath_get_subname", Fox_icall_NodePath_get_subname);
	GDMonoUtils::add_internal_call("Fox.NodePath::Fox_icall_NodePath_get_subname_count", Fox_icall_NodePath_get_subname_count);
	GDMonoUtils::add_internal_call("Fox.NodePath::Fox_icall_NodePath_is_absolute", Fox_icall_NodePath_is_absolute);
	GDMonoUtils::add_internal_call("Fox.NodePath::Fox_icall_NodePath_is_empty", Fox_icall_NodePath_is_empty);
}

#endif // MONO_GLUE_ENABLED
