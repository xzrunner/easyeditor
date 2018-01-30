#ifndef _S2LOADER_SPINE_ANIM_LOADER_H_
#define _S2LOADER_SPINE_ANIM_LOADER_H_

#include "s2loader/SpineParser.h"

#include <cu/uncopyable.h>
#include <sprite2/JointPose.h>
#include <cu/cu_stl.h>

#include <json/json.h>

#include <memory>

namespace s2 { class AnimSymbol; class SkeletonPose; class SkeletonSymbol; }

namespace s2loader
{

class SymbolLoader;
class SpriteLoader;

class SpineAnimLoader : private cu::Uncopyable
{
public:
	SpineAnimLoader(s2::AnimSymbol& sym, 
		const std::shared_ptr<const SymbolLoader>& sym_loader = nullptr,
		const std::shared_ptr<const SpriteLoader>& spr_loader = nullptr);

	void LoadJson(const Json::Value& val, const CU_STR& dir,
		const CU_STR& filepath);

private:
	void BuildBone2PoseTable();

	float GetNextTime();
	void LoadJointPoses(float next_time, s2::SkeletonPose& sk_pose);
	void UpdateNextTime(float next_time);

private:
	s2::AnimSymbol& m_sym;

	std::shared_ptr<const SymbolLoader> m_sym_loader;
	std::shared_ptr<const SpriteLoader> m_spr_loader;

	const SpineParser::Animation* m_src_anim;

	std::shared_ptr<s2::SkeletonSymbol> m_sk_sym = nullptr;

	// table for bone to pose index
	CU_VEC<int> m_bone2pose;

	// index to current each pose
	CU_VEC<int> m_pose_ptrs;

}; // SpineAnimLoader

}

#endif // _S2LOADER_SPINE_ANIM_LOADER_H_
