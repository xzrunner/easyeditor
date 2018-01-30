#include "s2loader/MeshIO.h"
#include "s2loader/SkeletonIO.h"

#include <SM_Vector.h>
#include <polymesh/MeshTransform.h>
#include <sprite2/Mesh.h>
#include <gum/JsonSerializer.h>

#include <queue>

#include <assert.h>

namespace s2loader
{

/************************************************************************/
/* MeshTransform                                                        */
/************************************************************************/

void MeshIO::Load(const Json::Value& val, pm::MeshTransform& trans, const s2::Mesh& mesh)
{
	if (val.isMember("trans"))
	{
		CU_VEC<sm::vec2> from, to;
		gum::JsonSerializer::Load(val["trans"]["from"], from);
		gum::JsonSerializer::Load(val["trans"]["to"], to);
		assert(from.size() == to.size());

		CU_VEC<sm::vec2> vertices, texcoords;
		CU_VEC<int> triangles;
		mesh.DumpToTriangles(vertices, texcoords, triangles);

		CU_VEC<std::pair<int, sm::vec2> > t;
		for (int i = 0, n = from.size(); i < n; ++i) {
			bool succ = false;
			for (int j = 0, m = vertices.size(); j < m; ++j) {
				if (from[i] == vertices[j]) {
					t.push_back(std::make_pair(j, to[i] - from[i]));
					succ = true;
					break;
				}
			}
			assert(succ);
		}
		trans.SetTrans(t);
	}
	else
	{
		const Json::Value& trans_val = val["transform"];
		CU_VEC<std::pair<int, sm::vec2> > t;
		for (int i = 0, n = trans_val["idx"].size(); i < n; ++i) 
		{
			int idx = trans_val["idx"][i].asInt();
			float x = static_cast<float>(trans_val["off_x"][i].asDouble()),
				  y = static_cast<float>(trans_val["off_y"][i].asDouble());
			t.push_back(std::make_pair(idx, sm::vec2(x, y)));
		}
		trans.SetTrans(t);
	}
}

void MeshIO::Store(Json::Value& val, const pm::MeshTransform& trans, const s2::Mesh& mesh)
{
	Json::Value& trans_val = val["transform"];
	const CU_VEC<std::pair<int, sm::vec2> >& t = trans.GetTrans();
	for (int i = 0, n = t.size(); i < n; ++i) {
		trans_val["idx"][i] = t[i].first;
		trans_val["off_x"][i] = t[i].second.x;
		trans_val["off_y"][i] = t[i].second.y;
	}
}

///************************************************************************/
///* MeshSkeleton                                                         */
///************************************************************************/
//
//void MeshIO::Load(const Json::Value& val, s2::MeshSkeleton& sk)
//{
//	struct Joint
//	{
//		s2::JointPose world;
//		int parent;
//	};
//
//	CU_VEC<Joint> joints;
//	joints.reserve(val["joint"].size());
//	for (int i = 0, n = val["joint"].size(); i < n; ++i)
//	{
//		const Json::Value& src = val["joint"][i];
//		Joint dst;
//		SkeletonIO::Load(src["world_pose"], dst.world);
//		dst.parent = src["parent"].asInt();
//		joints.push_back(dst);
//	}
//
//	CU_VEC<s2::MeshJoint*> mjoints;
//	mjoints.reserve(joints.size());
//	s2::MeshJoint* root = nullptr;
//	for (int i = 0, n = joints.size(); i < n; ++i)
//	{
//		const Joint& src = joints[i];
//		if (src.parent == -1) {
//			mjoints.push_back(root = new s2::MeshJoint(nullptr, src.world));
//		} else {
//			assert(src.parent < mjoints.size());
//			mjoints.push_back(new s2::MeshJoint(mjoints[src.parent], src.world));
//		}
//	}
//
//	if (root) {
//		sk.SetRoot(root);
//	}
//}
//
//void MeshIO::Store(Json::Value& val, const s2::MeshSkeleton& trans)
//{
//	const s2::MeshJoint* root = trans.GetRoot();
//	if (!root) {
//		return;
//	}
//
//	CU_VEC<const s2::MeshJoint*> joints;
//	std::queue<const s2::MeshJoint*> buf;
//	buf.push(root);
//	while (!buf.empty()) {
//		const s2::MeshJoint* joint = buf.front(); buf.pop();
//		joints.push_back(joint);
//		const CU_VEC<s2::MeshJoint*>& children = joint->GetChildren();
//		for (int i = 0, n = children.size(); i < n; ++i) {
//			buf.push(children[i]);
//		}
//	}
//
//	for (int i = 0, n = joints.size(); i < n; ++i) 
//	{
//		const s2::MeshJoint* joint = joints[i];
//		Json::Value cval;
//		SkeletonIO::Store(cval["world_pose"], joint->GetWorldPose());
//		int parent = -1;
//		for (int idx = 0; idx < n; ++idx) {
//			if (joints[idx] == joint->GetParent()) {
//				parent = idx;
//				break;
//			}
//		}
//		cval["parent"] = parent;
//		val["joint"][i] = cval;
//	}
//}

}