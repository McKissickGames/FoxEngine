/*************************************************************************/
/*  glue_header.h                                                        */
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

#include "../mono_gd/gd_mono_marshal.h"

void Fox_register_collections_icalls();
void Fox_register_gd_icalls();
void Fox_register_string_name_icalls();
void Fox_register_nodepath_icalls();
void Fox_register_object_icalls();
void Fox_register_rid_icalls();
void Fox_register_string_icalls();
void Fox_register_scene_tree_icalls();

/**
 * Registers internal calls that were not generated. This function is called
 * from the generated FoxSharpBindings::register_generated_icalls() function.
 */
void Fox_register_glue_header_icalls() {
	Fox_register_collections_icalls();
	Fox_register_gd_icalls();
	Fox_register_string_name_icalls();
	Fox_register_nodepath_icalls();
	Fox_register_object_icalls();
	Fox_register_rid_icalls();
	Fox_register_string_icalls();
	Fox_register_scene_tree_icalls();
}

// Used by the generated glue

#include "core/config/engine.h"
#include "core/object/class_db.h"
#include "core/object/method_bind.h"
#include "core/object/ref_counted.h"
#include "core/string/node_path.h"
#include "core/string/ustring.h"
#include "core/typedefs.h"
#include "core/variant/array.h"
#include "core/variant/dictionary.h"

#include "../mono_gd/gd_mono_class.h"
#include "../mono_gd/gd_mono_internals.h"
#include "../mono_gd/gd_mono_utils.h"

#define FoxSHARP_INSTANCE_OBJECT(m_instance, m_type) \
	static ClassDB::ClassInfo *ci = nullptr;           \
	if (!ci) {                                         \
		ci = ClassDB::classes.getptr(m_type);          \
	}                                                  \
	Object *m_instance = ci->creation_func();

#include "arguments_vector.h"

#endif // MONO_GLUE_ENABLED
