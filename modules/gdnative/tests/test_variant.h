/*************************************************************************/
/*  test_variant.h                                                       */
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

#ifndef TEST_GDNATIVE_VARIANT_H
#define TEST_GDNATIVE_VARIANT_H

#include <gdnative/gdnative.h>
#include <gdnative/variant.h>

#include "tests/test_macros.h"

namespace TestGDNativeVariant {

TEST_CASE("[GDNative Variant] New Variant with copy") {
	Fox_variant src;
	Fox_variant_new_int(&src, 42);

	Fox_variant copy;
	Fox_variant_new_copy(&copy, &src);

	CHECK(Fox_variant_as_int(&copy) == 42);
	CHECK(Fox_variant_get_type(&copy) == Fox_VARIANT_TYPE_INT);

	Fox_variant_destroy(&src);
	Fox_variant_destroy(&copy);
}

TEST_CASE("[GDNative Variant] New Variant with Nil") {
	Fox_variant val;
	Fox_variant_new_nil(&val);

	CHECK(Fox_variant_get_type(&val) == Fox_VARIANT_TYPE_NIL);

	Fox_variant_destroy(&val);
}

TEST_CASE("[GDNative Variant] New Variant with bool") {
	Fox_variant val;
	Fox_variant_new_bool(&val, true);

	CHECK(Fox_variant_as_bool(&val));
	CHECK(Fox_variant_get_type(&val) == Fox_VARIANT_TYPE_BOOL);

	Fox_variant_destroy(&val);
}

TEST_CASE("[GDNative Variant] New Variant with float") {
	Fox_variant val;
	Fox_variant_new_float(&val, 4.2);

	CHECK(Fox_variant_as_float(&val) == 4.2);
	CHECK(Fox_variant_get_type(&val) == Fox_VARIANT_TYPE_FLOAT);

	Fox_variant_destroy(&val);
}

TEST_CASE("[GDNative Variant] New Variant with String") {
	String str = "something";

	Fox_variant val;
	Fox_variant_new_string(&val, (Fox_string *)&str);
	Fox_string gd_str = Fox_variant_as_string(&val);
	String *result = (String *)&gd_str;

	CHECK(*result == String("something"));
	CHECK(Fox_variant_get_type(&val) == Fox_VARIANT_TYPE_STRING);

	Fox_variant_destroy(&val);
	Fox_string_destroy(&gd_str);
}

TEST_CASE("[GDNative Variant] Variant call") {
	String str("something");
	Fox_variant self;
	Fox_variant_new_string(&self, (Fox_string *)&str);

	Fox_variant ret;

	Fox_string_name method;
	Fox_string_name_new_with_latin1_chars(&method, "is_valid_identifier");

	Fox_variant_call_error error;
	Fox_variant_call(&self, &method, nullptr, 0, &ret, &error);

	CHECK(Fox_variant_get_type(&ret) == Fox_VARIANT_TYPE_BOOL);
	CHECK(Fox_variant_as_bool(&ret));

	Fox_variant_destroy(&ret);
	Fox_variant_destroy(&self);
	Fox_string_name_destroy(&method);
}

TEST_CASE("[GDNative Variant] Variant evaluate") {
	Fox_variant one;
	Fox_variant_new_int(&one, 1);
	Fox_variant two;
	Fox_variant_new_int(&two, 2);

	Fox_variant three;
	Fox_variant_new_nil(&three);
	bool valid = false;

	Fox_variant_evaluate(Fox_VARIANT_OP_ADD, &one, &two, &three, &valid);

	CHECK(Fox_variant_get_type(&three) == Fox_VARIANT_TYPE_INT);
	CHECK(Fox_variant_as_int(&three) == 3);
	CHECK(valid);

	Fox_variant_destroy(&one);
	Fox_variant_destroy(&two);
	Fox_variant_destroy(&three);
}

TEST_CASE("[GDNative Variant] Variant set/get named") {
	Fox_string_name x;
	Fox_string_name_new_with_latin1_chars(&x, "x");

	Vector2 vec(0, 0);
	Fox_variant self;
	Fox_variant_new_vector2(&self, (Fox_vector2 *)&vec);

	Fox_variant set;
	Fox_variant_new_float(&set, 1.0);

	bool set_valid = false;
	Fox_variant_set_named(&self, &x, &set, &set_valid);

	bool get_valid = false;
	Fox_variant get = Fox_variant_get_named(&self, &x, &get_valid);

	CHECK(get_valid);
	CHECK(set_valid);
	CHECK(Fox_variant_get_type(&get) == Fox_VARIANT_TYPE_FLOAT);
	CHECK(Fox_variant_as_float(&get) == 1.0);

	Fox_string_name_destroy(&x);
	Fox_variant_destroy(&self);
	Fox_variant_destroy(&set);
	Fox_variant_destroy(&get);
}

TEST_CASE("[GDNative Variant] Get utility function argument name") {
	Fox_string_name function;
	Fox_string_name_new_with_latin1_chars(&function, "pow");

	Fox_string arg_name = Fox_variant_get_utility_function_argument_name(&function, 0);

	String *arg_name_str = (String *)&arg_name;

	CHECK(*arg_name_str == "base");

	Fox_string_destroy(&arg_name);
	Fox_string_name_destroy(&function);
}

TEST_CASE("[GDNative Variant] Get utility function list") {
	int count = Fox_variant_get_utility_function_count();

	Fox_string_name *c_list = (Fox_string_name *)Fox_alloc(count * sizeof(Fox_string_name));
	Fox_variant_get_utility_function_list(c_list);

	List<StringName> cpp_list;
	Variant::get_utility_function_list(&cpp_list);

	Fox_string_name *cur = c_list;

	for (const List<StringName>::Element *E = cpp_list.front(); E; E = E->next()) {
		const StringName &cpp_name = E->get();
		StringName *c_name = (StringName *)cur++;

		CHECK(*c_name == cpp_name);
	}

	Fox_free(c_list);
}
} // namespace TestGDNativeVariant

#endif // TEST_GDNATIVE_VARIANT_H
