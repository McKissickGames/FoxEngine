/*************************************************************************/
/*  gltf_skeleton.h                                                      */
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

#ifndef GLTF_SKELETON_H
#define GLTF_SKELETON_H

#include "core/io/resource.h"
#include "gltf_document.h"

class GLTFSkeleton : public Resource {
	GDCLASS(GLTFSkeleton, Resource);
	friend class GLTFDocument;

private:
	// The *synthesized* skeletons joints
	Vector<GLTFNodeIndex> joints;

	// The roots of the skeleton. If there are multiple, each root must have the
	// same parent (ie roots are siblings)
	Vector<GLTFNodeIndex> roots;

	// The created Skeleton3D for the scene
	Skeleton3D *Fox_skeleton = nullptr;

	// Set of unique bone names for the skeleton
	Set<String> unique_names;

	Map<int32_t, GLTFNodeIndex> Fox_bone_node;

	Vector<BoneAttachment3D *> bone_attachments;

protected:
	static void _bind_methods();

public:
	Vector<GLTFNodeIndex> get_joints();
	void set_joints(Vector<GLTFNodeIndex> p_joints);

	Vector<GLTFNodeIndex> get_roots();
	void set_roots(Vector<GLTFNodeIndex> p_roots);

	Skeleton3D *get_Fox_skeleton();

	// Skeleton *get_Fox_skeleton() {
	// 	return this->Fox_skeleton;
	// }
	// void set_Fox_skeleton(Skeleton p_*Fox_skeleton) {
	// 	this->Fox_skeleton = p_Fox_skeleton;
	// }

	Array get_unique_names();
	void set_unique_names(Array p_unique_names);

	//Map<int32_t, GLTFNodeIndex> get_Fox_bone_node() {
	//	return this->Fox_bone_node;
	//}
	//void set_Fox_bone_node(Map<int32_t, GLTFNodeIndex> p_Fox_bone_node) {
	//	this->Fox_bone_node = p_Fox_bone_node;
	//}
	Dictionary get_Fox_bone_node();
	void set_Fox_bone_node(Dictionary p_indict);

	//Dictionary get_Fox_bone_node() {
	//	return VariantConversion::to_dict(this->Fox_bone_node);
	//}
	//void set_Fox_bone_node(Dictionary p_indict) {
	//	VariantConversion::set_from_dict(this->Fox_bone_node, p_indict);
	//}

	BoneAttachment3D *get_bone_attachment(int idx);

	int32_t get_bone_attachment_count();
};
#endif // GLTF_SKELETON_H
