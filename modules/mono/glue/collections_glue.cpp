/*************************************************************************/
/*  collections_glue.cpp                                                 */
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

#include <mono/metadata/exception.h>

#include "core/variant/array.h"

#include "../mono_gd/gd_mono_cache.h"
#include "../mono_gd/gd_mono_class.h"
#include "../mono_gd/gd_mono_marshal.h"
#include "../mono_gd/gd_mono_utils.h"

Array *Fox_icall_Array_Ctor() {
	return memnew(Array);
}

void Fox_icall_Array_Dtor(Array *ptr) {
	memdelete(ptr);
}

MonoObject *Fox_icall_Array_At(Array *ptr, int32_t index) {
	if (index < 0 || index >= ptr->size()) {
		GDMonoUtils::set_pending_exception(mono_get_exception_index_out_of_range());
		return nullptr;
	}
	return GDMonoMarshal::variant_to_mono_object(ptr->operator[](index));
}

MonoObject *Fox_icall_Array_At_Generic(Array *ptr, int32_t index, uint32_t type_encoding, GDMonoClass *type_class) {
	if (index < 0 || index >= ptr->size()) {
		GDMonoUtils::set_pending_exception(mono_get_exception_index_out_of_range());
		return nullptr;
	}
	return GDMonoMarshal::variant_to_mono_object(ptr->operator[](index), ManagedType(type_encoding, type_class));
}

void Fox_icall_Array_SetAt(Array *ptr, int32_t index, MonoObject *value) {
	if (index < 0 || index >= ptr->size()) {
		GDMonoUtils::set_pending_exception(mono_get_exception_index_out_of_range());
		return;
	}
	ptr->operator[](index) = GDMonoMarshal::mono_object_to_variant(value);
}

int32_t Fox_icall_Array_Count(Array *ptr) {
	return ptr->size();
}

int32_t Fox_icall_Array_Add(Array *ptr, MonoObject *item) {
	ptr->append(GDMonoMarshal::mono_object_to_variant(item));
	return ptr->size();
}

void Fox_icall_Array_Clear(Array *ptr) {
	ptr->clear();
}

MonoBoolean Fox_icall_Array_Contains(Array *ptr, MonoObject *item) {
	return ptr->find(GDMonoMarshal::mono_object_to_variant(item)) != -1;
}

void Fox_icall_Array_CopyTo(Array *ptr, MonoArray *array, int32_t array_index) {
	unsigned int count = ptr->size();

	if (mono_array_length(array) < (array_index + count)) {
		MonoException *exc = mono_get_exception_argument("", "Destination array was not long enough. Check destIndex and length, and the array's lower bounds.");
		GDMonoUtils::set_pending_exception(exc);
		return;
	}

	for (unsigned int i = 0; i < count; i++) {
		MonoObject *boxed = GDMonoMarshal::variant_to_mono_object(ptr->operator[](i));
		mono_array_setref(array, array_index, boxed);
		array_index++;
	}
}

Array *Fox_icall_Array_Ctor_MonoArray(MonoArray *mono_array) {
	Array *Fox_array = memnew(Array);
	unsigned int count = mono_array_length(mono_array);
	Fox_array->resize(count);
	for (unsigned int i = 0; i < count; i++) {
		MonoObject *item = mono_array_get(mono_array, MonoObject *, i);
		Fox_icall_Array_SetAt(Fox_array, i, item);
	}
	return Fox_array;
}

Array *Fox_icall_Array_Duplicate(Array *ptr, MonoBoolean deep) {
	return memnew(Array(ptr->duplicate(deep)));
}

Array *Fox_icall_Array_Concatenate(Array *left, Array *right) {
	int count = left->size() + right->size();
	Array *new_array = memnew(Array(left->duplicate(false)));
	new_array->resize(count);
	for (unsigned int i = 0; i < (unsigned int)right->size(); i++) {
		new_array->operator[](i + left->size()) = right->operator[](i);
	}
	return new_array;
}

int32_t Fox_icall_Array_IndexOf(Array *ptr, MonoObject *item) {
	return ptr->find(GDMonoMarshal::mono_object_to_variant(item));
}

void Fox_icall_Array_Insert(Array *ptr, int32_t index, MonoObject *item) {
	if (index < 0 || index > ptr->size()) {
		GDMonoUtils::set_pending_exception(mono_get_exception_index_out_of_range());
		return;
	}
	ptr->insert(index, GDMonoMarshal::mono_object_to_variant(item));
}

MonoBoolean Fox_icall_Array_Remove(Array *ptr, MonoObject *item) {
	int idx = ptr->find(GDMonoMarshal::mono_object_to_variant(item));
	if (idx >= 0) {
		ptr->remove(idx);
		return true;
	}
	return false;
}

void Fox_icall_Array_RemoveAt(Array *ptr, int32_t index) {
	if (index < 0 || index >= ptr->size()) {
		GDMonoUtils::set_pending_exception(mono_get_exception_index_out_of_range());
		return;
	}
	ptr->remove(index);
}

int32_t Fox_icall_Array_Resize(Array *ptr, int32_t new_size) {
	return (int32_t)ptr->resize(new_size);
}

void Fox_icall_Array_Shuffle(Array *ptr) {
	ptr->shuffle();
}

void Fox_icall_Array_Generic_GetElementTypeInfo(MonoReflectionType *refltype, uint32_t *type_encoding, GDMonoClass **type_class) {
	MonoType *elem_type = mono_reflection_type_get_type(refltype);

	*type_encoding = mono_type_get_type(elem_type);
	MonoClass *type_class_raw = mono_class_from_mono_type(elem_type);
	*type_class = GDMono::get_singleton()->get_class(type_class_raw);
}

MonoString *Fox_icall_Array_ToString(Array *ptr) {
	return GDMonoMarshal::mono_string_from_Fox(Variant(*ptr).operator String());
}

Dictionary *Fox_icall_Dictionary_Ctor() {
	return memnew(Dictionary);
}

void Fox_icall_Dictionary_Dtor(Dictionary *ptr) {
	memdelete(ptr);
}

MonoObject *Fox_icall_Dictionary_GetValue(Dictionary *ptr, MonoObject *key) {
	Variant *ret = ptr->getptr(GDMonoMarshal::mono_object_to_variant(key));
	if (ret == nullptr) {
		MonoObject *exc = mono_object_new(mono_domain_get(), CACHED_CLASS(KeyNotFoundException)->get_mono_ptr());
#ifdef DEBUG_ENABLED
		CRASH_COND(!exc);
#endif
		GDMonoUtils::runtime_object_init(exc, CACHED_CLASS(KeyNotFoundException));
		GDMonoUtils::set_pending_exception((MonoException *)exc);
		return nullptr;
	}
	return GDMonoMarshal::variant_to_mono_object(ret);
}

MonoObject *Fox_icall_Dictionary_GetValue_Generic(Dictionary *ptr, MonoObject *key, uint32_t type_encoding, GDMonoClass *type_class) {
	Variant *ret = ptr->getptr(GDMonoMarshal::mono_object_to_variant(key));
	if (ret == nullptr) {
		MonoObject *exc = mono_object_new(mono_domain_get(), CACHED_CLASS(KeyNotFoundException)->get_mono_ptr());
#ifdef DEBUG_ENABLED
		CRASH_COND(!exc);
#endif
		GDMonoUtils::runtime_object_init(exc, CACHED_CLASS(KeyNotFoundException));
		GDMonoUtils::set_pending_exception((MonoException *)exc);
		return nullptr;
	}
	return GDMonoMarshal::variant_to_mono_object(ret, ManagedType(type_encoding, type_class));
}

void Fox_icall_Dictionary_SetValue(Dictionary *ptr, MonoObject *key, MonoObject *value) {
	ptr->operator[](GDMonoMarshal::mono_object_to_variant(key)) = GDMonoMarshal::mono_object_to_variant(value);
}

Array *Fox_icall_Dictionary_Keys(Dictionary *ptr) {
	return memnew(Array(ptr->keys()));
}

Array *Fox_icall_Dictionary_Values(Dictionary *ptr) {
	return memnew(Array(ptr->values()));
}

int32_t Fox_icall_Dictionary_Count(Dictionary *ptr) {
	return ptr->size();
}

void Fox_icall_Dictionary_Add(Dictionary *ptr, MonoObject *key, MonoObject *value) {
	Variant varKey = GDMonoMarshal::mono_object_to_variant(key);
	Variant *ret = ptr->getptr(varKey);
	if (ret != nullptr) {
		GDMonoUtils::set_pending_exception(mono_get_exception_argument("key", "An element with the same key already exists"));
		return;
	}
	ptr->operator[](varKey) = GDMonoMarshal::mono_object_to_variant(value);
}

void Fox_icall_Dictionary_Clear(Dictionary *ptr) {
	ptr->clear();
}

MonoBoolean Fox_icall_Dictionary_Contains(Dictionary *ptr, MonoObject *key, MonoObject *value) {
	// no dupes
	Variant *ret = ptr->getptr(GDMonoMarshal::mono_object_to_variant(key));
	return ret != nullptr && *ret == GDMonoMarshal::mono_object_to_variant(value);
}

MonoBoolean Fox_icall_Dictionary_ContainsKey(Dictionary *ptr, MonoObject *key) {
	return ptr->has(GDMonoMarshal::mono_object_to_variant(key));
}

Dictionary *Fox_icall_Dictionary_Duplicate(Dictionary *ptr, MonoBoolean deep) {
	return memnew(Dictionary(ptr->duplicate(deep)));
}

MonoBoolean Fox_icall_Dictionary_RemoveKey(Dictionary *ptr, MonoObject *key) {
	return ptr->erase(GDMonoMarshal::mono_object_to_variant(key));
}

MonoBoolean Fox_icall_Dictionary_Remove(Dictionary *ptr, MonoObject *key, MonoObject *value) {
	Variant varKey = GDMonoMarshal::mono_object_to_variant(key);

	// no dupes
	Variant *ret = ptr->getptr(varKey);
	if (ret != nullptr && *ret == GDMonoMarshal::mono_object_to_variant(value)) {
		ptr->erase(varKey);
		return true;
	}

	return false;
}

MonoBoolean Fox_icall_Dictionary_TryGetValue(Dictionary *ptr, MonoObject *key, MonoObject **value) {
	Variant *ret = ptr->getptr(GDMonoMarshal::mono_object_to_variant(key));
	if (ret == nullptr) {
		*value = nullptr;
		return false;
	}
	*value = GDMonoMarshal::variant_to_mono_object(ret);
	return true;
}

MonoBoolean Fox_icall_Dictionary_TryGetValue_Generic(Dictionary *ptr, MonoObject *key, MonoObject **value, uint32_t type_encoding, GDMonoClass *type_class) {
	Variant *ret = ptr->getptr(GDMonoMarshal::mono_object_to_variant(key));
	if (ret == nullptr) {
		*value = nullptr;
		return false;
	}
	*value = GDMonoMarshal::variant_to_mono_object(ret, ManagedType(type_encoding, type_class));
	return true;
}

void Fox_icall_Dictionary_Generic_GetValueTypeInfo(MonoReflectionType *refltype, uint32_t *type_encoding, GDMonoClass **type_class) {
	MonoType *value_type = mono_reflection_type_get_type(refltype);

	*type_encoding = mono_type_get_type(value_type);
	MonoClass *type_class_raw = mono_class_from_mono_type(value_type);
	*type_class = GDMono::get_singleton()->get_class(type_class_raw);
}

MonoString *Fox_icall_Dictionary_ToString(Dictionary *ptr) {
	return GDMonoMarshal::mono_string_from_Fox(Variant(*ptr).operator String());
}

void Fox_register_collections_icalls() {
	GDMonoUtils::add_internal_call("Fox.Collections.Array::Fox_icall_Array_Ctor", Fox_icall_Array_Ctor);
	GDMonoUtils::add_internal_call("Fox.Collections.Array::Fox_icall_Array_Ctor_MonoArray", Fox_icall_Array_Ctor_MonoArray);
	GDMonoUtils::add_internal_call("Fox.Collections.Array::Fox_icall_Array_Dtor", Fox_icall_Array_Dtor);
	GDMonoUtils::add_internal_call("Fox.Collections.Array::Fox_icall_Array_At", Fox_icall_Array_At);
	GDMonoUtils::add_internal_call("Fox.Collections.Array::Fox_icall_Array_At_Generic", Fox_icall_Array_At_Generic);
	GDMonoUtils::add_internal_call("Fox.Collections.Array::Fox_icall_Array_SetAt", Fox_icall_Array_SetAt);
	GDMonoUtils::add_internal_call("Fox.Collections.Array::Fox_icall_Array_Count", Fox_icall_Array_Count);
	GDMonoUtils::add_internal_call("Fox.Collections.Array::Fox_icall_Array_Add", Fox_icall_Array_Add);
	GDMonoUtils::add_internal_call("Fox.Collections.Array::Fox_icall_Array_Clear", Fox_icall_Array_Clear);
	GDMonoUtils::add_internal_call("Fox.Collections.Array::Fox_icall_Array_Concatenate", Fox_icall_Array_Concatenate);
	GDMonoUtils::add_internal_call("Fox.Collections.Array::Fox_icall_Array_Contains", Fox_icall_Array_Contains);
	GDMonoUtils::add_internal_call("Fox.Collections.Array::Fox_icall_Array_CopyTo", Fox_icall_Array_CopyTo);
	GDMonoUtils::add_internal_call("Fox.Collections.Array::Fox_icall_Array_Duplicate", Fox_icall_Array_Duplicate);
	GDMonoUtils::add_internal_call("Fox.Collections.Array::Fox_icall_Array_IndexOf", Fox_icall_Array_IndexOf);
	GDMonoUtils::add_internal_call("Fox.Collections.Array::Fox_icall_Array_Insert", Fox_icall_Array_Insert);
	GDMonoUtils::add_internal_call("Fox.Collections.Array::Fox_icall_Array_Remove", Fox_icall_Array_Remove);
	GDMonoUtils::add_internal_call("Fox.Collections.Array::Fox_icall_Array_RemoveAt", Fox_icall_Array_RemoveAt);
	GDMonoUtils::add_internal_call("Fox.Collections.Array::Fox_icall_Array_Resize", Fox_icall_Array_Resize);
	GDMonoUtils::add_internal_call("Fox.Collections.Array::Fox_icall_Array_Shuffle", Fox_icall_Array_Shuffle);
	GDMonoUtils::add_internal_call("Fox.Collections.Array::Fox_icall_Array_Generic_GetElementTypeInfo", Fox_icall_Array_Generic_GetElementTypeInfo);
	GDMonoUtils::add_internal_call("Fox.Collections.Array::Fox_icall_Array_ToString", Fox_icall_Array_ToString);

	GDMonoUtils::add_internal_call("Fox.Collections.Dictionary::Fox_icall_Dictionary_Ctor", Fox_icall_Dictionary_Ctor);
	GDMonoUtils::add_internal_call("Fox.Collections.Dictionary::Fox_icall_Dictionary_Dtor", Fox_icall_Dictionary_Dtor);
	GDMonoUtils::add_internal_call("Fox.Collections.Dictionary::Fox_icall_Dictionary_GetValue", Fox_icall_Dictionary_GetValue);
	GDMonoUtils::add_internal_call("Fox.Collections.Dictionary::Fox_icall_Dictionary_GetValue_Generic", Fox_icall_Dictionary_GetValue_Generic);
	GDMonoUtils::add_internal_call("Fox.Collections.Dictionary::Fox_icall_Dictionary_SetValue", Fox_icall_Dictionary_SetValue);
	GDMonoUtils::add_internal_call("Fox.Collections.Dictionary::Fox_icall_Dictionary_Keys", Fox_icall_Dictionary_Keys);
	GDMonoUtils::add_internal_call("Fox.Collections.Dictionary::Fox_icall_Dictionary_Values", Fox_icall_Dictionary_Values);
	GDMonoUtils::add_internal_call("Fox.Collections.Dictionary::Fox_icall_Dictionary_Count", Fox_icall_Dictionary_Count);
	GDMonoUtils::add_internal_call("Fox.Collections.Dictionary::Fox_icall_Dictionary_Add", Fox_icall_Dictionary_Add);
	GDMonoUtils::add_internal_call("Fox.Collections.Dictionary::Fox_icall_Dictionary_Clear", Fox_icall_Dictionary_Clear);
	GDMonoUtils::add_internal_call("Fox.Collections.Dictionary::Fox_icall_Dictionary_Contains", Fox_icall_Dictionary_Contains);
	GDMonoUtils::add_internal_call("Fox.Collections.Dictionary::Fox_icall_Dictionary_ContainsKey", Fox_icall_Dictionary_ContainsKey);
	GDMonoUtils::add_internal_call("Fox.Collections.Dictionary::Fox_icall_Dictionary_Duplicate", Fox_icall_Dictionary_Duplicate);
	GDMonoUtils::add_internal_call("Fox.Collections.Dictionary::Fox_icall_Dictionary_RemoveKey", Fox_icall_Dictionary_RemoveKey);
	GDMonoUtils::add_internal_call("Fox.Collections.Dictionary::Fox_icall_Dictionary_Remove", Fox_icall_Dictionary_Remove);
	GDMonoUtils::add_internal_call("Fox.Collections.Dictionary::Fox_icall_Dictionary_TryGetValue", Fox_icall_Dictionary_TryGetValue);
	GDMonoUtils::add_internal_call("Fox.Collections.Dictionary::Fox_icall_Dictionary_TryGetValue_Generic", Fox_icall_Dictionary_TryGetValue_Generic);
	GDMonoUtils::add_internal_call("Fox.Collections.Dictionary::Fox_icall_Dictionary_Generic_GetValueTypeInfo", Fox_icall_Dictionary_Generic_GetValueTypeInfo);
	GDMonoUtils::add_internal_call("Fox.Collections.Dictionary::Fox_icall_Dictionary_ToString", Fox_icall_Dictionary_ToString);
}

#endif // MONO_GLUE_ENABLED
