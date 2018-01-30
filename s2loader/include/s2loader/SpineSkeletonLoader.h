#ifndef _S2LOADER_SPINE_SKELETON_LOADER_H_
#define _S2LOADER_SPINE_SKELETON_LOADER_H_

#include <cu/uncopyable.h>
#include <sprite2/typedef.h>
#include <cu/cu_stl.h>

#include <json/value.h>

namespace s2 { class SkeletonSymbol; class Sprite; class Joint; }

namespace s2loader
{

class SpriteLoader;
class JointLoader;
class SpineParser;

class SpineSkeletonLoader : private cu::Uncopyable
{
public:
	SpineSkeletonLoader(s2::SkeletonSymbol& sym, 
		const std::shared_ptr<const SpriteLoader>& spr_loader = nullptr,
		const std::shared_ptr<const JointLoader>& joint_loader = nullptr);

	void LoadJson(const Json::Value& val, const CU_STR& dir);

	void LoadParser(const SpineParser& parser, const CU_STR& dir);

private:
	void Clear();

	void LoadSprites(const SpineParser& parser, const CU_STR& img_dir);
	void LoadJoints(const SpineParser& parser);
	void InitRoot();
	void InitPose(const SpineParser& parser);
	void InitSkeleton(const SpineParser& parser);

private:
	s2::SkeletonSymbol& m_sym;

	std::shared_ptr<const SpriteLoader> m_spr_loader;
	std::shared_ptr<const JointLoader> m_joint_loader;

	int m_num;
	CU_VEC<s2::SprPtr> m_sprs;
	CU_MAP<CU_STR, std::shared_ptr<s2::Joint>> m_joints;
	std::shared_ptr<s2::Joint> m_root;

}; // SpineSkeletonLoader

}

#endif // _S2LOADER_SPINE_SKELETON_LOADER_H_
