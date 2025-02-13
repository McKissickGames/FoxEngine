/*************************************************************************/
/*  gd_mono_cache.cpp                                                    */
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

#include "gd_mono_cache.h"

#include "gd_mono.h"
#include "gd_mono_class.h"
#include "gd_mono_marshal.h"
#include "gd_mono_method.h"
#include "gd_mono_utils.h"

namespace GDMonoCache {

CachedData cached_data;

#define CACHE_AND_CHECK(m_var, m_val)                                                  \
	{                                                                                  \
		CRASH_COND(m_var != nullptr);                                                  \
		m_var = m_val;                                                                 \
		ERR_FAIL_COND_MSG(m_var == nullptr, "Mono Cache: Member " #m_var " is null."); \
	}

#define CACHE_CLASS_AND_CHECK(m_class, m_val) CACHE_AND_CHECK(cached_data.class_##m_class, m_val)
#define CACHE_NS_CLASS_AND_CHECK(m_ns, m_class, m_val) CACHE_AND_CHECK(cached_data.class_##m_ns##_##m_class, m_val)
#define CACHE_RAW_MONO_CLASS_AND_CHECK(m_class, m_val) CACHE_AND_CHECK(cached_data.rawclass_##m_class, m_val)
#define CACHE_FIELD_AND_CHECK(m_class, m_field, m_val) CACHE_AND_CHECK(cached_data.field_##m_class##_##m_field, m_val)
#define CACHE_METHOD_AND_CHECK(m_class, m_method, m_val) CACHE_AND_CHECK(cached_data.method_##m_class##_##m_method, m_val)
#define CACHE_PROPERTY_AND_CHECK(m_class, m_property, m_val) CACHE_AND_CHECK(cached_data.property_##m_class##_##m_property, m_val)

#define CACHE_METHOD_THUNK_AND_CHECK_IMPL(m_var, m_val)                                           \
	{                                                                                             \
		CRASH_COND(!m_var.is_null());                                                             \
		ERR_FAIL_COND_MSG(m_val == nullptr, "Mono Cache: Method for member " #m_var " is null."); \
		m_var.set_from_method(m_val);                                                             \
		ERR_FAIL_COND_MSG(m_var.is_null(), "Mono Cache: Member " #m_var " is null.");             \
	}

#define CACHE_METHOD_THUNK_AND_CHECK(m_class, m_method, m_val) CACHE_METHOD_THUNK_AND_CHECK_IMPL(cached_data.methodthunk_##m_class##_##m_method, m_val)

void CachedData::clear_corlib_cache() {
	corlib_cache_updated = false;

	class_MonoObject = nullptr;
	class_bool = nullptr;
	class_int8_t = nullptr;
	class_int16_t = nullptr;
	class_int32_t = nullptr;
	class_int64_t = nullptr;
	class_uint8_t = nullptr;
	class_uint16_t = nullptr;
	class_uint32_t = nullptr;
	class_uint64_t = nullptr;
	class_float = nullptr;
	class_double = nullptr;
	class_String = nullptr;
	class_IntPtr = nullptr;

	class_System_Collections_IEnumerable = nullptr;
	class_System_Collections_ICollection = nullptr;
	class_System_Collections_IDictionary = nullptr;

#ifdef DEBUG_ENABLED
	class_System_Diagnostics_StackTrace = nullptr;
	methodthunk_System_Diagnostics_StackTrace_GetFrames.nullify();
	method_System_Diagnostics_StackTrace_ctor_bool = nullptr;
	method_System_Diagnostics_StackTrace_ctor_Exception_bool = nullptr;
#endif

	class_KeyNotFoundException = nullptr;
}

void CachedData::clear_Fox_api_cache() {
	Fox_api_cache_updated = false;

	rawclass_Dictionary = nullptr;

	class_Vector2 = nullptr;
	class_Vector2i = nullptr;
	class_Rect2 = nullptr;
	class_Rect2i = nullptr;
	class_Transform2D = nullptr;
	class_Vector3 = nullptr;
	class_Vector3i = nullptr;
	class_Basis = nullptr;
	class_Quaternion = nullptr;
	class_Transform3D = nullptr;
	class_AABB = nullptr;
	class_Color = nullptr;
	class_Plane = nullptr;
	class_StringName = nullptr;
	class_NodePath = nullptr;
	class_RID = nullptr;
	class_FoxObject = nullptr;
	class_FoxResource = nullptr;
	class_Node = nullptr;
	class_Control = nullptr;
	class_Node3D = nullptr;
	class_WeakRef = nullptr;
	class_Callable = nullptr;
	class_SignalInfo = nullptr;
	class_Array = nullptr;
	class_Dictionary = nullptr;
	class_MarshalUtils = nullptr;
	class_ISerializationListener = nullptr;

#ifdef DEBUG_ENABLED
	class_DebuggingUtils = nullptr;
	methodthunk_DebuggingUtils_GetStackFrameInfo.nullify();
#endif

	class_ExportAttribute = nullptr;
	field_ExportAttribute_hint = nullptr;
	field_ExportAttribute_hintString = nullptr;
	class_SignalAttribute = nullptr;
	class_ToolAttribute = nullptr;
	class_RemoteAttribute = nullptr;
	class_MasterAttribute = nullptr;
	class_PuppetAttribute = nullptr;
	class_RemoteSyncAttribute = nullptr;
	class_MasterSyncAttribute = nullptr;
	class_PuppetSyncAttribute = nullptr;
	class_FoxMethodAttribute = nullptr;
	field_FoxMethodAttribute_methodName = nullptr;
	class_ScriptPathAttribute = nullptr;
	field_ScriptPathAttribute_path = nullptr;
	class_AssemblyHasScriptsAttribute = nullptr;
	field_AssemblyHasScriptsAttribute_requiresLookup = nullptr;
	field_AssemblyHasScriptsAttribute_scriptTypes = nullptr;

	field_FoxObject_ptr = nullptr;
	field_StringName_ptr = nullptr;
	field_NodePath_ptr = nullptr;
	field_Image_ptr = nullptr;
	field_RID_ptr = nullptr;

	methodthunk_FoxObject_Dispose.nullify();
	methodthunk_Array_GetPtr.nullify();
	methodthunk_Dictionary_GetPtr.nullify();
	methodthunk_SignalAwaiter_SignalCallback.nullify();
	methodthunk_FoxTaskScheduler_Activate.nullify();

	methodthunk_Delegate_Equals.nullify();

	methodthunk_DelegateUtils_TrySerializeDelegate.nullify();
	methodthunk_DelegateUtils_TryDeserializeDelegate.nullify();

	// Start of MarshalUtils methods

	methodthunk_MarshalUtils_TypeIsGenericArray.nullify();
	methodthunk_MarshalUtils_TypeIsGenericDictionary.nullify();
	methodthunk_MarshalUtils_TypeIsSystemGenericList.nullify();
	methodthunk_MarshalUtils_TypeIsSystemGenericDictionary.nullify();
	methodthunk_MarshalUtils_TypeIsGenericIEnumerable.nullify();
	methodthunk_MarshalUtils_TypeIsGenericICollection.nullify();
	methodthunk_MarshalUtils_TypeIsGenericIDictionary.nullify();

	methodthunk_MarshalUtils_ArrayGetElementType.nullify();
	methodthunk_MarshalUtils_DictionaryGetKeyValueTypes.nullify();

	methodthunk_MarshalUtils_MakeGenericArrayType.nullify();
	methodthunk_MarshalUtils_MakeGenericDictionaryType.nullify();

	// End of MarshalUtils methods

	task_scheduler_handle = Ref<MonoGCHandleRef>();
}

#define Fox_API_CLASS(m_class) (GDMono::get_singleton()->get_core_api_assembly()->get_class(BINDINGS_NAMESPACE, #m_class))
#define Fox_API_NS_CLASS(m_ns, m_class) (GDMono::get_singleton()->get_core_api_assembly()->get_class(m_ns, #m_class))

void update_corlib_cache() {
	CACHE_CLASS_AND_CHECK(MonoObject, GDMono::get_singleton()->get_corlib_assembly()->get_class(mono_get_object_class()));
	CACHE_CLASS_AND_CHECK(bool, GDMono::get_singleton()->get_corlib_assembly()->get_class(mono_get_boolean_class()));
	CACHE_CLASS_AND_CHECK(int8_t, GDMono::get_singleton()->get_corlib_assembly()->get_class(mono_get_sbyte_class()));
	CACHE_CLASS_AND_CHECK(int16_t, GDMono::get_singleton()->get_corlib_assembly()->get_class(mono_get_int16_class()));
	CACHE_CLASS_AND_CHECK(int32_t, GDMono::get_singleton()->get_corlib_assembly()->get_class(mono_get_int32_class()));
	CACHE_CLASS_AND_CHECK(int64_t, GDMono::get_singleton()->get_corlib_assembly()->get_class(mono_get_int64_class()));
	CACHE_CLASS_AND_CHECK(uint8_t, GDMono::get_singleton()->get_corlib_assembly()->get_class(mono_get_byte_class()));
	CACHE_CLASS_AND_CHECK(uint16_t, GDMono::get_singleton()->get_corlib_assembly()->get_class(mono_get_uint16_class()));
	CACHE_CLASS_AND_CHECK(uint32_t, GDMono::get_singleton()->get_corlib_assembly()->get_class(mono_get_uint32_class()));
	CACHE_CLASS_AND_CHECK(uint64_t, GDMono::get_singleton()->get_corlib_assembly()->get_class(mono_get_uint64_class()));
	CACHE_CLASS_AND_CHECK(float, GDMono::get_singleton()->get_corlib_assembly()->get_class(mono_get_single_class()));
	CACHE_CLASS_AND_CHECK(double, GDMono::get_singleton()->get_corlib_assembly()->get_class(mono_get_double_class()));
	CACHE_CLASS_AND_CHECK(String, GDMono::get_singleton()->get_corlib_assembly()->get_class(mono_get_string_class()));
	CACHE_CLASS_AND_CHECK(IntPtr, GDMono::get_singleton()->get_corlib_assembly()->get_class(mono_get_intptr_class()));

	CACHE_CLASS_AND_CHECK(System_Collections_IEnumerable, GDMono::get_singleton()->get_corlib_assembly()->get_class("System.Collections", "IEnumerable"));
	CACHE_CLASS_AND_CHECK(System_Collections_ICollection, GDMono::get_singleton()->get_corlib_assembly()->get_class("System.Collections", "ICollection"));
	CACHE_CLASS_AND_CHECK(System_Collections_IDictionary, GDMono::get_singleton()->get_corlib_assembly()->get_class("System.Collections", "IDictionary"));

#ifdef DEBUG_ENABLED
	CACHE_CLASS_AND_CHECK(System_Diagnostics_StackTrace, GDMono::get_singleton()->get_corlib_assembly()->get_class("System.Diagnostics", "StackTrace"));
	CACHE_METHOD_THUNK_AND_CHECK(System_Diagnostics_StackTrace, GetFrames, CACHED_CLASS(System_Diagnostics_StackTrace)->get_method("GetFrames"));
	CACHE_METHOD_AND_CHECK(System_Diagnostics_StackTrace, ctor_bool, CACHED_CLASS(System_Diagnostics_StackTrace)->get_method_with_desc("System.Diagnostics.StackTrace:.ctor(bool)", true));
	CACHE_METHOD_AND_CHECK(System_Diagnostics_StackTrace, ctor_Exception_bool, CACHED_CLASS(System_Diagnostics_StackTrace)->get_method_with_desc("System.Diagnostics.StackTrace:.ctor(System.Exception,bool)", true));
#endif

	CACHE_METHOD_THUNK_AND_CHECK(Delegate, Equals, GDMono::get_singleton()->get_corlib_assembly()->get_class("System", "Delegate")->get_method_with_desc("System.Delegate:Equals(object)", true));

	CACHE_CLASS_AND_CHECK(KeyNotFoundException, GDMono::get_singleton()->get_corlib_assembly()->get_class("System.Collections.Generic", "KeyNotFoundException"));

	cached_data.corlib_cache_updated = true;
}

void update_Fox_api_cache() {
	CACHE_CLASS_AND_CHECK(Vector2, Fox_API_CLASS(Vector2));
	CACHE_CLASS_AND_CHECK(Vector2i, Fox_API_CLASS(Vector2i));
	CACHE_CLASS_AND_CHECK(Rect2, Fox_API_CLASS(Rect2));
	CACHE_CLASS_AND_CHECK(Rect2i, Fox_API_CLASS(Rect2i));
	CACHE_CLASS_AND_CHECK(Transform2D, Fox_API_CLASS(Transform2D));
	CACHE_CLASS_AND_CHECK(Vector3, Fox_API_CLASS(Vector3));
	CACHE_CLASS_AND_CHECK(Vector3i, Fox_API_CLASS(Vector3i));
	CACHE_CLASS_AND_CHECK(Basis, Fox_API_CLASS(Basis));
	CACHE_CLASS_AND_CHECK(Quaternion, Fox_API_CLASS(Quaternion));
	CACHE_CLASS_AND_CHECK(Transform3D, Fox_API_CLASS(Transform3D));
	CACHE_CLASS_AND_CHECK(AABB, Fox_API_CLASS(AABB));
	CACHE_CLASS_AND_CHECK(Color, Fox_API_CLASS(Color));
	CACHE_CLASS_AND_CHECK(Plane, Fox_API_CLASS(Plane));
	CACHE_CLASS_AND_CHECK(StringName, Fox_API_CLASS(StringName));
	CACHE_CLASS_AND_CHECK(NodePath, Fox_API_CLASS(NodePath));
	CACHE_CLASS_AND_CHECK(RID, Fox_API_CLASS(RID));
	CACHE_CLASS_AND_CHECK(FoxObject, Fox_API_CLASS(Object));
	CACHE_CLASS_AND_CHECK(FoxResource, Fox_API_CLASS(Resource));
	CACHE_CLASS_AND_CHECK(Node, Fox_API_CLASS(Node));
	CACHE_CLASS_AND_CHECK(Control, Fox_API_CLASS(Control));
	CACHE_CLASS_AND_CHECK(Node3D, Fox_API_CLASS(Node3D));
	CACHE_CLASS_AND_CHECK(WeakRef, Fox_API_CLASS(WeakRef));
	CACHE_CLASS_AND_CHECK(Callable, Fox_API_CLASS(Callable));
	CACHE_CLASS_AND_CHECK(SignalInfo, Fox_API_CLASS(SignalInfo));
	CACHE_CLASS_AND_CHECK(Array, Fox_API_NS_CLASS(BINDINGS_NAMESPACE_COLLECTIONS, Array));
	CACHE_CLASS_AND_CHECK(Dictionary, Fox_API_NS_CLASS(BINDINGS_NAMESPACE_COLLECTIONS, Dictionary));
	CACHE_CLASS_AND_CHECK(MarshalUtils, Fox_API_CLASS(MarshalUtils));
	CACHE_CLASS_AND_CHECK(ISerializationListener, Fox_API_CLASS(ISerializationListener));

#ifdef DEBUG_ENABLED
	CACHE_CLASS_AND_CHECK(DebuggingUtils, Fox_API_CLASS(DebuggingUtils));
#endif

	// Attributes
	CACHE_CLASS_AND_CHECK(ExportAttribute, Fox_API_CLASS(ExportAttribute));
	CACHE_FIELD_AND_CHECK(ExportAttribute, hint, CACHED_CLASS(ExportAttribute)->get_field("hint"));
	CACHE_FIELD_AND_CHECK(ExportAttribute, hintString, CACHED_CLASS(ExportAttribute)->get_field("hintString"));
	CACHE_CLASS_AND_CHECK(SignalAttribute, Fox_API_CLASS(SignalAttribute));
	CACHE_CLASS_AND_CHECK(ToolAttribute, Fox_API_CLASS(ToolAttribute));
	CACHE_CLASS_AND_CHECK(RemoteAttribute, Fox_API_CLASS(RemoteAttribute));
	CACHE_CLASS_AND_CHECK(MasterAttribute, Fox_API_CLASS(MasterAttribute));
	CACHE_CLASS_AND_CHECK(PuppetAttribute, Fox_API_CLASS(PuppetAttribute));
	CACHE_CLASS_AND_CHECK(RemoteSyncAttribute, Fox_API_CLASS(RemoteSyncAttribute));
	CACHE_CLASS_AND_CHECK(MasterSyncAttribute, Fox_API_CLASS(MasterSyncAttribute));
	CACHE_CLASS_AND_CHECK(PuppetSyncAttribute, Fox_API_CLASS(PuppetSyncAttribute));
	CACHE_CLASS_AND_CHECK(FoxMethodAttribute, Fox_API_CLASS(FoxMethodAttribute));
	CACHE_FIELD_AND_CHECK(FoxMethodAttribute, methodName, CACHED_CLASS(FoxMethodAttribute)->get_field("methodName"));
	CACHE_CLASS_AND_CHECK(ScriptPathAttribute, Fox_API_CLASS(ScriptPathAttribute));
	CACHE_FIELD_AND_CHECK(ScriptPathAttribute, path, CACHED_CLASS(ScriptPathAttribute)->get_field("path"));
	CACHE_CLASS_AND_CHECK(AssemblyHasScriptsAttribute, Fox_API_CLASS(AssemblyHasScriptsAttribute));
	CACHE_FIELD_AND_CHECK(AssemblyHasScriptsAttribute, requiresLookup, CACHED_CLASS(AssemblyHasScriptsAttribute)->get_field("requiresLookup"));
	CACHE_FIELD_AND_CHECK(AssemblyHasScriptsAttribute, scriptTypes, CACHED_CLASS(AssemblyHasScriptsAttribute)->get_field("scriptTypes"));

	CACHE_FIELD_AND_CHECK(FoxObject, ptr, CACHED_CLASS(FoxObject)->get_field(BINDINGS_PTR_FIELD));
	CACHE_FIELD_AND_CHECK(StringName, ptr, CACHED_CLASS(StringName)->get_field(BINDINGS_PTR_FIELD));
	CACHE_FIELD_AND_CHECK(NodePath, ptr, CACHED_CLASS(NodePath)->get_field(BINDINGS_PTR_FIELD));
	CACHE_FIELD_AND_CHECK(RID, ptr, CACHED_CLASS(RID)->get_field(BINDINGS_PTR_FIELD));

	CACHE_METHOD_THUNK_AND_CHECK(FoxObject, Dispose, CACHED_CLASS(FoxObject)->get_method("Dispose", 0));
	CACHE_METHOD_THUNK_AND_CHECK(Array, GetPtr, Fox_API_NS_CLASS(BINDINGS_NAMESPACE_COLLECTIONS, Array)->get_method("GetPtr", 0));
	CACHE_METHOD_THUNK_AND_CHECK(Dictionary, GetPtr, Fox_API_NS_CLASS(BINDINGS_NAMESPACE_COLLECTIONS, Dictionary)->get_method("GetPtr", 0));
	CACHE_METHOD_THUNK_AND_CHECK(SignalAwaiter, SignalCallback, Fox_API_CLASS(SignalAwaiter)->get_method("SignalCallback", 1));
	CACHE_METHOD_THUNK_AND_CHECK(FoxTaskScheduler, Activate, Fox_API_CLASS(FoxTaskScheduler)->get_method("Activate", 0));

	CACHE_METHOD_THUNK_AND_CHECK(DelegateUtils, TrySerializeDelegate, Fox_API_CLASS(DelegateUtils)->get_method("TrySerializeDelegate", 2));
	CACHE_METHOD_THUNK_AND_CHECK(DelegateUtils, TryDeserializeDelegate, Fox_API_CLASS(DelegateUtils)->get_method("TryDeserializeDelegate", 2));

	// Start of MarshalUtils methods

	CACHE_METHOD_THUNK_AND_CHECK(MarshalUtils, TypeIsGenericArray, Fox_API_CLASS(MarshalUtils)->get_method("TypeIsGenericArray", 1));
	CACHE_METHOD_THUNK_AND_CHECK(MarshalUtils, TypeIsGenericDictionary, Fox_API_CLASS(MarshalUtils)->get_method("TypeIsGenericDictionary", 1));
	CACHE_METHOD_THUNK_AND_CHECK(MarshalUtils, TypeIsSystemGenericList, Fox_API_CLASS(MarshalUtils)->get_method("TypeIsSystemGenericList", 1));
	CACHE_METHOD_THUNK_AND_CHECK(MarshalUtils, TypeIsSystemGenericDictionary, Fox_API_CLASS(MarshalUtils)->get_method("TypeIsSystemGenericDictionary", 1));
	CACHE_METHOD_THUNK_AND_CHECK(MarshalUtils, TypeIsGenericIEnumerable, Fox_API_CLASS(MarshalUtils)->get_method("TypeIsGenericIEnumerable", 1));
	CACHE_METHOD_THUNK_AND_CHECK(MarshalUtils, TypeIsGenericICollection, Fox_API_CLASS(MarshalUtils)->get_method("TypeIsGenericICollection", 1));
	CACHE_METHOD_THUNK_AND_CHECK(MarshalUtils, TypeIsGenericIDictionary, Fox_API_CLASS(MarshalUtils)->get_method("TypeIsGenericIDictionary", 1));

	CACHE_METHOD_THUNK_AND_CHECK(MarshalUtils, ArrayGetElementType, Fox_API_CLASS(MarshalUtils)->get_method("ArrayGetElementType", 2));
	CACHE_METHOD_THUNK_AND_CHECK(MarshalUtils, DictionaryGetKeyValueTypes, Fox_API_CLASS(MarshalUtils)->get_method("DictionaryGetKeyValueTypes", 3));

	CACHE_METHOD_THUNK_AND_CHECK(MarshalUtils, MakeGenericArrayType, Fox_API_CLASS(MarshalUtils)->get_method("MakeGenericArrayType", 1));
	CACHE_METHOD_THUNK_AND_CHECK(MarshalUtils, MakeGenericDictionaryType, Fox_API_CLASS(MarshalUtils)->get_method("MakeGenericDictionaryType", 2));

	// End of MarshalUtils methods

#ifdef DEBUG_ENABLED
	CACHE_METHOD_THUNK_AND_CHECK(DebuggingUtils, GetStackFrameInfo, Fox_API_CLASS(DebuggingUtils)->get_method("GetStackFrameInfo", 4));
#endif

	// TODO Move to CSharpLanguage::init() and do handle disposal
	MonoObject *task_scheduler = mono_object_new(mono_domain_get(), Fox_API_CLASS(FoxTaskScheduler)->get_mono_ptr());
	GDMonoUtils::runtime_object_init(task_scheduler, Fox_API_CLASS(FoxTaskScheduler));
	cached_data.task_scheduler_handle = MonoGCHandleRef::create_strong(task_scheduler);

	cached_data.Fox_api_cache_updated = true;
}
} // namespace GDMonoCache
