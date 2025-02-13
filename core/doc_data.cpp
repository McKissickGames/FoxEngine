/*************************************************************************/
/*  doc_data.cpp                                                         */
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

#include "doc_data.h"

void DocData::return_doc_from_retinfo(DocData::MethodDoc &p_method, const PropertyInfo &p_retinfo) {
	if (p_retinfo.type == Variant::INT && p_retinfo.usage & PROPERTY_USAGE_CLASS_IS_ENUM) {
		p_method.return_enum = p_retinfo.class_name;
		if (p_method.return_enum.begins_with("_")) { //proxy class
			p_method.return_enum = p_method.return_enum.substr(1, p_method.return_enum.length());
		}
		p_method.return_type = "int";
	} else if (p_retinfo.class_name != StringName()) {
		p_method.return_type = p_retinfo.class_name;
	} else if (p_retinfo.type == Variant::ARRAY && p_retinfo.hint == PROPERTY_HINT_ARRAY_TYPE) {
		p_method.return_type = p_retinfo.hint_string + "[]";
	} else if (p_retinfo.hint == PROPERTY_HINT_RESOURCE_TYPE) {
		p_method.return_type = p_retinfo.hint_string;
	} else if (p_retinfo.type == Variant::NIL && p_retinfo.usage & PROPERTY_USAGE_NIL_IS_VARIANT) {
		p_method.return_type = "Variant";
	} else if (p_retinfo.type == Variant::NIL) {
		p_method.return_type = "void";
	} else {
		p_method.return_type = Variant::get_type_name(p_retinfo.type);
	}
}

void DocData::argument_doc_from_arginfo(DocData::ArgumentDoc &p_argument, const PropertyInfo &p_arginfo) {
	p_argument.name = p_arginfo.name;

	if (p_arginfo.type == Variant::INT && p_arginfo.usage & PROPERTY_USAGE_CLASS_IS_ENUM) {
		p_argument.enumeration = p_arginfo.class_name;
		if (p_argument.enumeration.begins_with("_")) { //proxy class
			p_argument.enumeration = p_argument.enumeration.substr(1, p_argument.enumeration.length());
		}
		p_argument.type = "int";
	} else if (p_arginfo.class_name != StringName()) {
		p_argument.type = p_arginfo.class_name;
	} else if (p_arginfo.type == Variant::ARRAY && p_arginfo.hint == PROPERTY_HINT_ARRAY_TYPE) {
		p_argument.type = p_arginfo.hint_string + "[]";
	} else if (p_arginfo.hint == PROPERTY_HINT_RESOURCE_TYPE) {
		p_argument.type = p_arginfo.hint_string;
	} else if (p_arginfo.type == Variant::NIL) {
		// Parameters cannot be void, so PROPERTY_USAGE_NIL_IS_VARIANT is not necessary
		p_argument.type = "Variant";
	} else {
		p_argument.type = Variant::get_type_name(p_arginfo.type);
	}
}

void DocData::property_doc_from_scriptmemberinfo(DocData::PropertyDoc &p_property, const ScriptMemberInfo &p_memberinfo) {
	p_property.name = p_memberinfo.propinfo.name;
	p_property.description = p_memberinfo.doc_string;

	if (p_memberinfo.propinfo.type == Variant::OBJECT) {
		p_property.type = p_memberinfo.propinfo.class_name;
	} else if (p_memberinfo.propinfo.type == Variant::NIL && p_memberinfo.propinfo.usage & PROPERTY_USAGE_NIL_IS_VARIANT) {
		p_property.type = "Variant";
	} else {
		p_property.type = Variant::get_type_name(p_memberinfo.propinfo.type);
	}

	p_property.setter = p_memberinfo.setter;
	p_property.getter = p_memberinfo.getter;

	if (p_memberinfo.has_default_value && p_memberinfo.default_value.get_type() != Variant::OBJECT) {
		p_property.default_value = p_memberinfo.default_value.get_construct_string().replace("\n", "");
	}

	p_property.overridden = false;
}

void DocData::method_doc_from_methodinfo(DocData::MethodDoc &p_method, const MethodInfo &p_methodinfo, const String &p_desc) {
	p_method.name = p_methodinfo.name;
	p_method.description = p_desc;

	return_doc_from_retinfo(p_method, p_methodinfo.return_val);

	for (int i = 0; i < p_methodinfo.arguments.size(); i++) {
		DocData::ArgumentDoc argument;
		argument_doc_from_arginfo(argument, p_methodinfo.arguments[i]);
		int default_arg_index = i - (p_methodinfo.arguments.size() - p_methodinfo.default_arguments.size());
		if (default_arg_index >= 0) {
			Variant default_arg = p_methodinfo.default_arguments[default_arg_index];
			argument.default_value = default_arg.get_construct_string();
		}
		p_method.arguments.push_back(argument);
	}
}

void DocData::constant_doc_from_variant(DocData::ConstantDoc &p_const, const StringName &p_name, const Variant &p_value, const String &p_desc) {
	p_const.name = p_name;
	p_const.value = p_value;
	p_const.description = p_desc;
}

void DocData::signal_doc_from_methodinfo(DocData::MethodDoc &p_signal, const MethodInfo &p_methodinfo, const String &p_desc) {
	return method_doc_from_methodinfo(p_signal, p_methodinfo, p_desc);
}
