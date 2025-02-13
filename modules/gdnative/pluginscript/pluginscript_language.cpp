/*************************************************************************/
/*  pluginscript_language.cpp                                            */
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

// Fox imports
#include "core/config/project_settings.h"
#include "core/io/file_access.h"
#include "core/os/os.h"
// PluginScript imports
#include "pluginscript_language.h"
#include "pluginscript_script.h"

String PluginScriptLanguage::get_name() const {
	return String(_desc.name);
}

void PluginScriptLanguage::init() {
	_data = _desc.init();
}

String PluginScriptLanguage::get_type() const {
	// We should use _desc.type here, however the returned type is used to
	// query ClassDB which would complain given the type is not registered
	// from his point of view...
	// To solve this we just use a more generic (but present in ClassDB) type.
	return String("PluginScript");
}

String PluginScriptLanguage::get_extension() const {
	return String(_desc.extension);
}

Error PluginScriptLanguage::execute_file(const String &p_path) {
	// TODO: pretty sure this method is totally deprecated and should be removed...
	return OK;
}

void PluginScriptLanguage::finish() {
	_desc.finish(_data);
}

/* EDITOR FUNCTIONS */

void PluginScriptLanguage::get_reserved_words(List<String> *p_words) const {
	if (_desc.reserved_words) {
		const char **w = _desc.reserved_words;
		while (*w) {
			p_words->push_back(*w);
			w++;
		}
	}
}

bool PluginScriptLanguage::is_control_flow_keyword(String p_keyword) const {
	return false;
}

void PluginScriptLanguage::get_comment_delimiters(List<String> *p_delimiters) const {
	if (_desc.comment_delimiters) {
		const char **w = _desc.comment_delimiters;
		while (*w) {
			p_delimiters->push_back(*w);
			w++;
		}
	}
}

void PluginScriptLanguage::get_string_delimiters(List<String> *p_delimiters) const {
	if (_desc.string_delimiters) {
		const char **w = _desc.string_delimiters;
		while (*w) {
			p_delimiters->push_back(*w);
			w++;
		}
	}
}

Ref<Script> PluginScriptLanguage::get_template(const String &p_class_name, const String &p_base_class_name) const {
	Script *ns = create_script();
	Ref<Script> script = Ref<Script>(ns);
	if (_desc.get_template_source_code) {
		Fox_string src = _desc.get_template_source_code(_data, (Fox_string *)&p_class_name, (Fox_string *)&p_base_class_name);
		script->set_source_code(*(String *)&src);
		Fox_string_destroy(&src);
	}
	return script;
}

bool PluginScriptLanguage::validate(const String &p_script, const String &p_path, List<String> *r_functions, List<ScriptLanguage::ScriptError> *r_errors, List<ScriptLanguage::Warning> *r_warnings, Set<int> *r_safe_lines) const {
	PackedStringArray functions;
	Array errors;
	if (_desc.validate) {
		bool ret = _desc.validate(
				_data,
				(Fox_string *)&p_script,
				(Fox_string *)&p_path,
				(Fox_packed_string_array *)&functions,
				(Fox_array *)&errors);
		for (int i = 0; i < functions.size(); i++) {
			r_functions->push_back(functions[i]);
		}
		if (r_errors) {
			for (int i = 0; i < errors.size(); i++) {
				Dictionary error = errors[i];
				ScriptLanguage::ScriptError e;
				e.line = error["line"];
				e.column = error["column"];
				e.message = error["message"];
				r_errors->push_back(e);
			}
		}
		return ret;
	}
	return true;
}

Script *PluginScriptLanguage::create_script() const {
	PluginScript *script = memnew(PluginScript());
	// I'm hurting kittens doing this I guess...
	script->init(const_cast<PluginScriptLanguage *>(this));
	return script;
}

bool PluginScriptLanguage::has_named_classes() const {
	return _desc.has_named_classes;
}

bool PluginScriptLanguage::supports_builtin_mode() const {
	return _desc.supports_builtin_mode;
}

bool PluginScriptLanguage::can_inherit_from_file() const {
	return _desc.can_inherit_from_file;
}

int PluginScriptLanguage::find_function(const String &p_function, const String &p_code) const {
	if (_desc.find_function) {
		return _desc.find_function(_data, (Fox_string *)&p_function, (Fox_string *)&p_code);
	}
	return -1;
}

String PluginScriptLanguage::make_function(const String &p_class, const String &p_name, const PackedStringArray &p_args) const {
	if (_desc.make_function) {
		Fox_string tmp = _desc.make_function(_data, (Fox_string *)&p_class, (Fox_string *)&p_name, (Fox_packed_string_array *)&p_args);
		String ret = *(String *)&tmp;
		Fox_string_destroy(&tmp);
		return ret;
	}
	return String();
}

Error PluginScriptLanguage::complete_code(const String &p_code, const String &p_path, Object *p_owner, List<ScriptCodeCompletionOption> *r_options, bool &r_force, String &r_call_hint) {
	if (_desc.complete_code) {
		Array options;
		Fox_error tmp = _desc.complete_code(
				_data,
				(Fox_string *)&p_code,
				(Fox_string *)&p_path,
				(Fox_object *)p_owner,
				(Fox_array *)&options,
				&r_force,
				(Fox_string *)&r_call_hint);
		for (int i = 0; i < options.size(); i++) {
			ScriptCodeCompletionOption option(options[i], ScriptCodeCompletionOption::KIND_PLAIN_TEXT);
			r_options->push_back(option);
		}
		return (Error)tmp;
	}
	return ERR_UNAVAILABLE;
}

void PluginScriptLanguage::auto_indent_code(String &p_code, int p_from_line, int p_to_line) const {
	if (_desc.auto_indent_code) {
		_desc.auto_indent_code(_data, (Fox_string *)&p_code, p_from_line, p_to_line);
	}
	return;
}

void PluginScriptLanguage::add_global_constant(const StringName &p_variable, const Variant &p_value) {
	_desc.add_global_constant(_data, (Fox_string_name *)&p_variable, (Fox_variant *)&p_value);
}

/* LOADER FUNCTIONS */

void PluginScriptLanguage::get_recognized_extensions(List<String> *p_extensions) const {
	for (int i = 0; _desc.recognized_extensions[i]; ++i) {
		p_extensions->push_back(String(_desc.recognized_extensions[i]));
	}
}

void PluginScriptLanguage::get_public_functions(List<MethodInfo> *p_functions) const {
	// TODO: provide this statically in `Fox_pluginscript_language_desc` ?
	if (_desc.get_public_functions) {
		Array functions;
		_desc.get_public_functions(_data, (Fox_array *)&functions);
		for (int i = 0; i < functions.size(); i++) {
			MethodInfo mi = MethodInfo::from_dict(functions[i]);
			p_functions->push_back(mi);
		}
	}
}

void PluginScriptLanguage::get_public_constants(List<Pair<String, Variant>> *p_constants) const {
	// TODO: provide this statically in `Fox_pluginscript_language_desc` ?
	if (_desc.get_public_constants) {
		Dictionary constants;
		_desc.get_public_constants(_data, (Fox_dictionary *)&constants);
		for (const Variant *key = constants.next(); key; key = constants.next(key)) {
			Variant value = constants[*key];
			p_constants->push_back(Pair<String, Variant>(*key, value));
		}
	}
}

void PluginScriptLanguage::profiling_start() {
#ifdef DEBUG_ENABLED
	if (_desc.profiling_start) {
		lock();
		_desc.profiling_start(_data);
		unlock();
	}
#endif
}

void PluginScriptLanguage::profiling_stop() {
#ifdef DEBUG_ENABLED
	if (_desc.profiling_stop) {
		lock();
		_desc.profiling_stop(_data);
		unlock();
	}
#endif
}

int PluginScriptLanguage::profiling_get_accumulated_data(ProfilingInfo *p_info_arr, int p_info_max) {
	int info_count = 0;
#ifdef DEBUG_ENABLED
	if (_desc.profiling_get_accumulated_data) {
		Fox_pluginscript_profiling_data *info = (Fox_pluginscript_profiling_data *)memalloc(
				sizeof(Fox_pluginscript_profiling_data) * p_info_max);
		info_count = _desc.profiling_get_accumulated_data(_data, info, p_info_max);
		for (int i = 0; i < info_count; ++i) {
			p_info_arr[i].signature = *(StringName *)&info[i].signature;
			p_info_arr[i].call_count = info[i].call_count;
			p_info_arr[i].total_time = info[i].total_time;
			p_info_arr[i].self_time = info[i].self_time;
			Fox_string_name_destroy(&info[i].signature);
		}
	}
#endif
	return info_count;
}

int PluginScriptLanguage::profiling_get_frame_data(ProfilingInfo *p_info_arr, int p_info_max) {
	int info_count = 0;
#ifdef DEBUG_ENABLED
	if (_desc.profiling_get_frame_data) {
		Fox_pluginscript_profiling_data *info = (Fox_pluginscript_profiling_data *)memalloc(
				sizeof(Fox_pluginscript_profiling_data) * p_info_max);
		info_count = _desc.profiling_get_frame_data(_data, info, p_info_max);
		for (int i = 0; i < info_count; ++i) {
			p_info_arr[i].signature = *(StringName *)&info[i].signature;
			p_info_arr[i].call_count = info[i].call_count;
			p_info_arr[i].total_time = info[i].total_time;
			p_info_arr[i].self_time = info[i].self_time;
			Fox_string_name_destroy(&info[i].signature);
		}
	}
#endif
	return info_count;
}

void PluginScriptLanguage::frame() {
#ifdef DEBUG_ENABLED
	if (_desc.profiling_frame) {
		_desc.profiling_frame(_data);
	}
#endif
}

/* DEBUGGER FUNCTIONS */

String PluginScriptLanguage::debug_get_error() const {
	if (_desc.debug_get_error) {
		Fox_string tmp = _desc.debug_get_error(_data);
		String ret = *(String *)&tmp;
		Fox_string_destroy(&tmp);
		return ret;
	}
	return String("Nothing");
}

int PluginScriptLanguage::debug_get_stack_level_count() const {
	if (_desc.debug_get_stack_level_count) {
		return _desc.debug_get_stack_level_count(_data);
	}
	return 1;
}

int PluginScriptLanguage::debug_get_stack_level_line(int p_level) const {
	if (_desc.debug_get_stack_level_line) {
		return _desc.debug_get_stack_level_line(_data, p_level);
	}
	return 1;
}

String PluginScriptLanguage::debug_get_stack_level_function(int p_level) const {
	if (_desc.debug_get_stack_level_function) {
		Fox_string tmp = _desc.debug_get_stack_level_function(_data, p_level);
		String ret = *(String *)&tmp;
		Fox_string_destroy(&tmp);
		return ret;
	}
	return String("Nothing");
}

String PluginScriptLanguage::debug_get_stack_level_source(int p_level) const {
	if (_desc.debug_get_stack_level_source) {
		Fox_string tmp = _desc.debug_get_stack_level_source(_data, p_level);
		String ret = *(String *)&tmp;
		Fox_string_destroy(&tmp);
		return ret;
	}
	return String("Nothing");
}

void PluginScriptLanguage::debug_get_stack_level_locals(int p_level, List<String> *p_locals, List<Variant> *p_values, int p_max_subitems, int p_max_depth) {
	if (_desc.debug_get_stack_level_locals) {
		PackedStringArray locals;
		Array values;
		_desc.debug_get_stack_level_locals(_data, p_level, (Fox_packed_string_array *)&locals, (Fox_array *)&values, p_max_subitems, p_max_depth);
		for (int i = 0; i < locals.size(); i++) {
			p_locals->push_back(locals[i]);
		}
		for (int i = 0; i < values.size(); i++) {
			p_values->push_back(values[i]);
		}
	}
}

void PluginScriptLanguage::debug_get_stack_level_members(int p_level, List<String> *p_members, List<Variant> *p_values, int p_max_subitems, int p_max_depth) {
	if (_desc.debug_get_stack_level_members) {
		PackedStringArray members;
		Array values;
		_desc.debug_get_stack_level_members(_data, p_level, (Fox_packed_string_array *)&members, (Fox_array *)&values, p_max_subitems, p_max_depth);
		for (int i = 0; i < members.size(); i++) {
			p_members->push_back(members[i]);
		}
		for (int i = 0; i < values.size(); i++) {
			p_values->push_back(values[i]);
		}
	}
}

void PluginScriptLanguage::debug_get_globals(List<String> *p_locals, List<Variant> *p_values, int p_max_subitems, int p_max_depth) {
	if (_desc.debug_get_globals) {
		PackedStringArray locals;
		Array values;
		_desc.debug_get_globals(_data, (Fox_packed_string_array *)&locals, (Fox_array *)&values, p_max_subitems, p_max_depth);
		for (int i = 0; i < locals.size(); i++) {
			p_locals->push_back(locals[i]);
		}
		for (int i = 0; i < values.size(); i++) {
			p_values->push_back(values[i]);
		}
	}
}

String PluginScriptLanguage::debug_parse_stack_level_expression(int p_level, const String &p_expression, int p_max_subitems, int p_max_depth) {
	if (_desc.debug_parse_stack_level_expression) {
		Fox_string tmp = _desc.debug_parse_stack_level_expression(_data, p_level, (Fox_string *)&p_expression, p_max_subitems, p_max_depth);
		String ret = *(String *)&tmp;
		Fox_string_destroy(&tmp);
		return ret;
	}
	return String("Nothing");
}

void PluginScriptLanguage::reload_all_scripts() {
	// TODO
}

void PluginScriptLanguage::reload_tool_script(const Ref<Script> &p_script, bool p_soft_reload) {
#ifdef DEBUG_ENABLED
	lock();
	// TODO
	unlock();
#endif
}

bool PluginScriptLanguage::handles_global_class_type(const String &p_type) const {
	return p_type == "PluginScript";
}

String PluginScriptLanguage::get_global_class_name(const String &p_path, String *r_base_type, String *r_icon_path) const {
	if (!p_path.is_empty()) {
		Ref<PluginScript> script = ResourceLoader::load(p_path, "PluginScript");
		if (script.is_valid()) {
			if (r_base_type) {
				*r_base_type = script->get_instance_base_type();
			}
			if (r_icon_path) {
				*r_icon_path = script->get_script_class_icon_path();
			}
			return script->get_script_class_name();
		}
		if (r_base_type) {
			*r_base_type = String();
		}
		if (r_icon_path) {
			*r_icon_path = String();
		}
	}
	return String();
}

void PluginScriptLanguage::lock() {
	_lock.lock();
}

void PluginScriptLanguage::unlock() {
	_lock.unlock();
}

PluginScriptLanguage::PluginScriptLanguage(const Fox_pluginscript_language_desc *desc) :
		_desc(*desc) {
	_resource_loader = Ref<ResourceFormatLoaderPluginScript>(memnew(ResourceFormatLoaderPluginScript(this)));
	_resource_saver = Ref<ResourceFormatSaverPluginScript>(memnew(ResourceFormatSaverPluginScript(this)));
}

PluginScriptLanguage::~PluginScriptLanguage() {
}
