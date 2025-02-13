/*************************************************************************/
/*  Foxsharp_dirs.h                                                    */
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

#ifndef FoxSHARP_DIRS_H
#define FoxSHARP_DIRS_H

#include "core/string/ustring.h"

namespace FoxSharpDirs {

String get_res_data_dir();
String get_res_metadata_dir();
String get_res_assemblies_base_dir();
String get_res_assemblies_dir();
String get_res_config_dir();
String get_res_temp_dir();
String get_res_temp_assemblies_base_dir();
String get_res_temp_assemblies_dir();

String get_mono_user_dir();
String get_mono_logs_dir();

#ifdef TOOLS_ENABLED
String get_mono_solutions_dir();
String get_build_logs_dir();

String get_project_sln_path();
String get_project_csproj_path();

String get_data_editor_tools_dir();
String get_data_editor_prebuilt_api_dir();
#else
String get_data_game_assemblies_dir();
#endif

String get_data_mono_etc_dir();
String get_data_mono_lib_dir();

#ifdef WINDOWS_ENABLED
String get_data_mono_bin_dir();
#endif
} // namespace FoxSharpDirs

#endif // FoxSHARP_DIRS_H
