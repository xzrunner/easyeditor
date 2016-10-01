#include "Symbol.h"
#include "Sprite.h"
#include "Joint.h"

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
		p.mt = spr->GetTransMatrix() * params.mt;
		p.color = spr->Color() * params.color;
	}

	const std::vector<s2::Joint*>& joints = m_skeleton->GetAllJoints();
	for (int i = 0, n = joints.size(); i < n; ++i) {
		s2::Joint* joint = joints[i];
		dynamic_cast<Joint*>(joint)->DrawSkeleton(p, joint == selected);
	}
}

void Symbol::LoadResources()
{
	class Loader : public gum::SkeletonSymLoader
	{
	public:
		Loader(s2::SkeletonSymbol* sym) : gum::SkeletonSymLoader(sym) {}
	protected:
		virtual s2::Joint* CreateJoint(s2::Sprite* spr, const s2::JointPose& joint_pose) const {
			return new Joint(spr, joint_pose);
		}
	}; // Loader

	Loader loader(this);
	loader.LoadJson(m_filepath);
}

}