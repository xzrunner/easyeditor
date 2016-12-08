#include "Symbol.h"
#include "Sprite.h"
#include "Joint.h"

#include <ee/SpriteLoader.h>

#include <sprite2/Skeleton.h>
#include <sprite2/RenderParams.h>
#include <gum/SkeletonSymLoader.h>

namespace libskeleton
{

Symbol::Symbol()
{
}

Symbol::~Symbol()
{
}

void Symbol::DrawSkeleton(const s2::RenderParams& params, const s2::Sprite* spr,
						  const s2::Joint* selected) const
{
	if (!m_skeleton) {
		return;
	}

	s2::RenderParams p = params;
	if (spr) {
		p.mt = spr->GetLocalMat() * params.mt;
		p.color = spr->GetColor() * params.color;
	}

	const std::vector<s2::Joint*>& joints = m_skeleton->GetAllJoints();
	for (int i = 0, n = joints.size(); i < n; ++i) {
		s2::Joint* joint = joints[i];
		dynamic_cast<Joint*>(joint)->DrawSkeleton(p, joint == selected);
	}
}

void Symbol::LoadResources()
{
	ee::SpriteLoader spr_loader;
	JointLoader joint_loader;
	gum::SkeletonSymLoader loader(this, &spr_loader, &joint_loader);
	loader.LoadJson(m_filepath);
}

/************************************************************************/
/* class Symbol::JointLoader                                            */
/************************************************************************/

s2::Joint* Symbol::JointLoader::Create(s2::Sprite* spr, const s2::JointPose& joint_pose) const 
{
	return new Joint(spr, joint_pose);
}

}