#ifndef _S2LOADER_SKELETON_SYM_LOADER_H_
#define _S2LOADER_SKELETON_SYM_LOADER_H_

#include <cu/uncopyable.h>
#include <SM_Vector.h>
#include <cu/cu_stl.h>

#include <json/json.h>

#include <memory>

namespace s2 { class SkeletonSymbol; class Joint; class Sprite; class JointPose; }

namespace s2loader
{

class SpriteLoader;
class JointLoader;

class SkeletonSymLoader : private cu::Uncopyable
{
public:
	SkeletonSymLoader(s2::SkeletonSymbol& sym, 
		const std::shared_ptr<const SpriteLoader>& spr_loader = nullptr,
		const std::shared_ptr<const JointLoader>& joint_loader = nullptr);

	void LoadJson(const CU_STR& filepath);

private:
	s2::SkeletonSymbol& m_sym;

	std::shared_ptr<const SpriteLoader> m_spr_loader;
	std::shared_ptr<const JointLoader>  m_joint_loader;

}; // SkeletonSymLoader

}

#endif // _S2LOADER_SKELETON_SYM_LOADER_H_
