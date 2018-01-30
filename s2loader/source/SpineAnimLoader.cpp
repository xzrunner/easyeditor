#include "s2loader/SpineAnimLoader.h"
#include "s2loader/SpineParser.h"
#include "s2loader/SymbolLoader.h"
#include "s2loader/SpriteLoader.h"

#include <sprite2/Sprite.h>
#include <sprite2/AnimSymbol.h>
#include <sprite2/SkeletonSymbol.h>
#include <sprite2/SkeletonSprite.h>
#include <sprite2/Joint.h>
#include <sprite2/Skeleton.h>
#include <gum/FilepathHelper.h>

#include <assert.h>

namespace s2loader
{

static const float FPS = 30.0f;
static const int DEFAULT_ANIM = 0;

SpineAnimLoader::SpineAnimLoader(s2::AnimSymbol& sym,
								 const std::shared_ptr<const SymbolLoader>& sym_loader,
								 const std::shared_ptr<const SpriteLoader>& spr_loader)
	: m_sym(sym)
	, m_sym_loader(sym_loader)
	, m_spr_loader(spr_loader)
	, m_src_anim(nullptr)
{
	if (!m_sym_loader) {
		m_sym_loader = std::make_shared<SymbolLoader>();
	}
	if (!m_spr_loader) {
		m_spr_loader = std::make_shared<SpriteLoader>();
	}
}

void SpineAnimLoader::LoadJson(const Json::Value& val, const CU_STR& dir,
							   const CU_STR& filepath)
{
	m_sym.SetFPS(static_cast<int>(FPS));

	SpineParser parser;
	parser.Parse(val);

	assert(!parser.anims.empty());
	m_src_anim = &parser.anims[DEFAULT_ANIM];

	m_sk_sym = S2_VI_PTR_DOWN_CAST<s2::SkeletonSymbol>(m_sym_loader->Create(filepath, s2::SYM_SKELETON));

	BuildBone2PoseTable();

	auto layer = CU_MAKE_UNIQUE<s2::AnimSymbol::Layer>();
	
	int bone_num = m_src_anim->bones.size();
	m_pose_ptrs.resize(bone_num * 3, 0);
	float next_time = 0;
	while (true)
	{
		float time = GetNextTime();
		if (next_time != 0 && time == next_time) {
			break;
		} else {
			next_time = time;
		}

		auto frame = CU_MAKE_UNIQUE<s2::AnimSymbol::Frame>();
		frame->index = static_cast<int>(next_time * FPS) + 1;
		frame->tween = true;
		auto spr = m_spr_loader->Create(m_sk_sym);
		spr->SetName("sk");
		LoadJointPoses(next_time, S2_VI_PTR_DOWN_CAST<s2::SkeletonSprite>(spr)->GetPose());
		frame->sprs.push_back(spr);
		layer->frames.push_back(std::move(frame));
		UpdateNextTime(next_time);
	}

	m_sym.AddLayer(layer);
}

void SpineAnimLoader::BuildBone2PoseTable()
{
	m_bone2pose.reserve(m_src_anim->bones.size());
	auto& sk = m_sk_sym->GetSkeleton();
	auto& all_joints = sk->GetAllJoints();
	for (int i = 0, n = m_src_anim->bones.size(); i < n; ++i) 
	{
		const CU_STR& name = m_src_anim->bones[i].name;
		int idx = -1;
		for (int j = 0, m = all_joints.size(); j < m; ++j) {
			if (all_joints[j]->GetName() == name) {
				idx = j;
				break;
			}
		}
		assert(idx != -1);
		m_bone2pose.push_back(idx);
	}
}

float SpineAnimLoader::GetNextTime()
{
	float next_time = FLT_MAX;
	int ptr = 0;
	int bone_num = m_src_anim->bones.size();
	for (int i = 0; i < bone_num; ++i)
	{
		const SpineParser::AnimBone& src_bone = m_src_anim->bones[i];
		// rotate
		if (!src_bone.rotates.empty()) 
		{
			int idx = m_pose_ptrs[ptr];
			if (idx < static_cast<int>(src_bone.rotates.size()) - 1)
			{
				float time = src_bone.rotates[idx + 1].time;
				if (time < next_time) {
					next_time = time;
				}
			}
		}
		++ptr;
		// translate
		if (!src_bone.translates.empty()) 
		{
			int idx = m_pose_ptrs[ptr];
			if (idx < static_cast<int>(src_bone.translates.size()) - 1)
			{
				float time = src_bone.translates[idx + 1].time;
				if (time < next_time) {
					next_time = time;
				}
			}
		}
		++ptr;
		// scale
		if (!src_bone.scales.empty()) 
		{
			int idx = m_pose_ptrs[ptr];
			if (idx < static_cast<int>(src_bone.scales.size()) - 1)
			{
				float time = src_bone.scales[idx + 1].time;
				if (time < next_time) {
					next_time = time;
				}
			}
		}
		++ptr;
	}
	return next_time;
}

void SpineAnimLoader::LoadJointPoses(float next_time, s2::SkeletonPose& sk_pose)
{
	auto& joints = m_sk_sym->GetSkeleton()->GetAllJoints();

	int ptr = 0;
	int bone_num = m_src_anim->bones.size();
	for (int i = 0; i < bone_num; ++i)
	{
		const SpineParser::AnimBone& src_bone = m_src_anim->bones[i];
		s2::JointPose pose;

		// rotate
		int idx = m_pose_ptrs[ptr++];
		if (src_bone.rotates.empty()) {
			;
		} else if (idx == src_bone.rotates.size() - 1) {
			pose.rot = src_bone.rotates[idx].rot;
		} else if (next_time >= src_bone.rotates[idx].time) {
			float t0 = src_bone.rotates[idx].time,
				  t1 = src_bone.rotates[idx + 1].time;
			assert(next_time <= t1);
			float v0 = src_bone.rotates[idx].rot,
				  v1 = src_bone.rotates[idx + 1].rot;
			pose.rot = (next_time - t0) * (v1 - v0) / (t1 - t0) + v0;			
		}
		// translate
		idx = m_pose_ptrs[ptr++];
		if (src_bone.translates.empty()) {
			;
		} else if (idx == src_bone.translates.size() - 1) {
			pose.trans = src_bone.translates[idx].trans;
		} else if (next_time >= src_bone.translates[idx].time) {
			float t0 = src_bone.translates[idx].time,
				  t1 = src_bone.translates[idx + 1].time;
			assert(next_time <= t1);
			sm::vec2 v0 = src_bone.translates[idx].trans,
				     v1 = src_bone.translates[idx + 1].trans;
			pose.trans = (v1 - v0) * (next_time - t0) / (t1 - t0) + v0;
		}
 		// scale
 		idx = m_pose_ptrs[ptr++];
		if (src_bone.scales.empty()) {
			;
		} else if (idx == src_bone.scales.size() - 1) {
			pose.scale = src_bone.scales[idx].scale;
		} else if (next_time >= src_bone.scales[idx].time) {
 			float t0 = src_bone.scales[idx].time,
 				  t1 = src_bone.scales[idx + 1].time;
 			assert(next_time <= t1);
 			sm::vec2 v0 = src_bone.scales[idx].scale,
 				     v1 = src_bone.scales[idx + 1].scale;
			pose.scale = (v1 - v0) * (next_time - t0) / (t1 - t0) + v0;
		}

		idx = m_bone2pose[i];
		const s2::JointPose& base = joints[idx]->GetLocalPose();
		pose.trans += base.trans;
		pose.rot += base.rot;
		sk_pose.SetJointPose(idx, pose);
	}
}

void SpineAnimLoader::UpdateNextTime(float next_time)
{
	int ptr = 0;
	int bone_num = m_src_anim->bones.size();
	for (int i = 0; i < bone_num; ++i)
	{
		const SpineParser::AnimBone& src_bone = m_src_anim->bones[i];
		// rotate
		if (!src_bone.rotates.empty()) 
		{
			int idx = m_pose_ptrs[ptr];
			if (idx < static_cast<int>(src_bone.rotates.size()) - 1) {
				if (src_bone.rotates[idx + 1].time <= next_time) {
					++m_pose_ptrs[ptr];
				}
			}
		}
		++ptr;
		// translate
		if (!src_bone.translates.empty()) 
		{
			int idx = m_pose_ptrs[ptr];
			if (idx < static_cast<int>(src_bone.translates.size()) - 1) {
				if (src_bone.translates[idx + 1].time <= next_time) {
					++m_pose_ptrs[ptr];
				}
			}
		}
		++ptr;
		// scale
		if (!src_bone.scales.empty()) 
		{
			int idx = m_pose_ptrs[ptr];
			if (idx < static_cast<int>(src_bone.scales.size()) - 1) {
				if (src_bone.scales[idx + 1].time <= next_time) {
					++m_pose_ptrs[ptr];
				}
			}
		}
		++ptr;
	}
}

}