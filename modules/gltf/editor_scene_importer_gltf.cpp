/*************************************************************************/
/*  editor_scene_importer_gltf.cpp                                       */
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

#include "editor_scene_importer_gltf.h"

#include "gltf_document.h"
#include "gltf_state.h"

#include "scene/3d/node_3d.h"
#include "scene/animation/animation_player.h"
#include "scene/resources/animation.h"

uint32_t EditorSceneImporterGLTF::get_import_flags() const {
	return ImportFlags::IMPORT_SCENE | ImportFlags::IMPORT_ANIMATION;
}

void EditorSceneImporterGLTF::get_extensions(List<String> *r_extensions) const {
	r_extensions->push_back("gltf");
	r_extensions->push_back("glb");
}

Node *EditorSceneImporterGLTF::import_scene(const String &p_path,
		uint32_t p_flags, int p_bake_fps,
		List<String> *r_missing_deps,
		Error *r_err) {
	Ref<PackedSceneGLTF> importer;
	importer.instantiate();
	return importer->import_scene(p_path, p_flags, p_bake_fps, r_missing_deps, r_err, Ref<GLTFState>());
}

Ref<Animation> EditorSceneImporterGLTF::import_animation(const String &p_path,
		uint32_t p_flags,
		int p_bake_fps) {
	return Ref<Animation>();
}

void PackedSceneGLTF::_bind_methods() {
	ClassDB::bind_method(
			D_METHOD("export_gltf", "node", "path", "flags", "bake_fps"),
			&PackedSceneGLTF::export_gltf, DEFVAL(0), DEFVAL(1000.0f));
	ClassDB::bind_method(D_METHOD("pack_gltf", "path", "flags", "bake_fps", "state"),
			&PackedSceneGLTF::pack_gltf, DEFVAL(0), DEFVAL(1000.0f), DEFVAL(Ref<GLTFState>()));
	ClassDB::bind_method(D_METHOD("import_gltf_scene", "path", "flags", "bake_fps", "state"),
			&PackedSceneGLTF::import_gltf_scene, DEFVAL(0), DEFVAL(1000.0f), DEFVAL(Ref<GLTFState>()));
}
Node *PackedSceneGLTF::import_gltf_scene(const String &p_path, uint32_t p_flags, float p_bake_fps, Ref<GLTFState> r_state) {
	Error err = FAILED;
	List<String> deps;
	return import_scene(p_path, p_flags, p_bake_fps, &deps, &err, r_state);
}

Node *PackedSceneGLTF::import_scene(const String &p_path, uint32_t p_flags,
		int p_bake_fps,
		List<String> *r_missing_deps,
		Error *r_err,
		Ref<GLTFState> r_state) {
	if (r_state == Ref<GLTFState>()) {
		r_state.instantiate();
	}
	r_state->use_named_skin_binds =
			p_flags & EditorSceneImporter::IMPORT_USE_NAMED_SKIN_BINDS;

	Ref<GLTFDocument> gltf_document;
	gltf_document.instantiate();
	Error err = gltf_document->parse(r_state, p_path);
	if (r_err) {
		*r_err = err;
	}
	ERR_FAIL_COND_V(err != Error::OK, nullptr);

	Node3D *root = memnew(Node3D);
	for (int32_t root_i = 0; root_i < r_state->root_nodes.size(); root_i++) {
		gltf_document->_generate_scene_node(r_state, root, root, r_state->root_nodes[root_i]);
	}
	gltf_document->_process_mesh_instances(r_state, root);
	if (r_state->animations.size()) {
		AnimationPlayer *ap = memnew(AnimationPlayer);
		root->add_child(ap);
		ap->set_owner(root);
		for (int i = 0; i < r_state->animations.size(); i++) {
			gltf_document->_import_animation(r_state, ap, i, p_bake_fps);
		}
	}

	return cast_to<Node3D>(root);
}

void PackedSceneGLTF::pack_gltf(String p_path, int32_t p_flags,
		real_t p_bake_fps, Ref<GLTFState> r_state) {
	Error err = FAILED;
	List<String> deps;
	Node *root = import_scene(p_path, p_flags, p_bake_fps, &deps, &err, r_state);
	ERR_FAIL_COND(err != OK);
	pack(root);
}

void PackedSceneGLTF::save_scene(Node *p_node, const String &p_path,
		const String &p_src_path, uint32_t p_flags,
		int p_bake_fps, List<String> *r_missing_deps,
		Error *r_err) {
	Error err = FAILED;
	if (r_err) {
		*r_err = err;
	}
	Ref<GLTFDocument> gltf_document;
	gltf_document.instantiate();
	Ref<GLTFState> state;
	state.instantiate();
	err = gltf_document->serialize(state, p_node, p_path);
	if (r_err) {
		*r_err = err;
	}
}

void PackedSceneGLTF::_build_parent_hierachy(Ref<GLTFState> state) {
	// build the hierarchy
	for (GLTFNodeIndex node_i = 0; node_i < state->nodes.size(); node_i++) {
		for (int j = 0; j < state->nodes[node_i]->children.size(); j++) {
			GLTFNodeIndex child_i = state->nodes[node_i]->children[j];
			ERR_FAIL_INDEX(child_i, state->nodes.size());
			if (state->nodes.write[child_i]->parent != -1) {
				continue;
			}
			state->nodes.write[child_i]->parent = node_i;
		}
	}
}

Error PackedSceneGLTF::export_gltf(Node *p_root, String p_path,
		int32_t p_flags,
		real_t p_bake_fps) {
	ERR_FAIL_COND_V(!p_root, FAILED);
	List<String> deps;
	Error err;
	String path = p_path;
	int32_t flags = p_flags;
	real_t baked_fps = p_bake_fps;
	Ref<PackedSceneGLTF> exporter;
	exporter.instantiate();
	exporter->save_scene(p_root, path, "", flags, baked_fps, &deps, &err);
	int32_t error_code = err;
	if (error_code != 0) {
		return Error(error_code);
	}
	return OK;
}
