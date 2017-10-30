#include "Symbol.h"
#include "Sprite.h"
#include "Joint.h"

#include <ee/SpriteLoader.h>

#include <sprite2/Skeleton.h>
#include <sprite2/RenderParams.h>
#include <gum/SkeletonSymLoader.h>
#include <gum/FilepathHelper.h>

namespace libskeleton
{

Symbol::Symbol()
{
}

Symbol::~Symbol()
{
}

void Symbol::DrawSkeleton(const s2::RenderParams& params, const s2::Sprite* spr,
						  const std::shared_ptr<const s2::Joint>& selected) const
{
	if (!m_skeleton) {
		return;
	}

	s2::RenderParams p = params;
	if (spr) {
		p.mt = spr->GetLocalMat() * params.mt;
		p.color = spr->GetColor() * params.color;
	}

	auto& joints = m_skeleton->GetAllJoints();
	for (int i = 0, n = joints.size(); i < n; ++i) {
		auto joint = joints[i];
		std::dynamic_pointer_cast<Joint>(joint)->DrawSkeleton(p, joint == selected);
	}
}

bool Symbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath.c_str())) {
		return false;
	}

	auto spr_loader(std::make_shared<ee::SpriteLoader>());
	auto joint_loader(std::make_shared<JointLoader>());
	gum::SkeletonSymLoader loader(*this, spr_loader, joint_loader);
	loader.LoadJson(m_filepath.c_str());

	return true;
}

/************************************************************************/
/* class Symbol::JointLoader                                            */
/************************************************************************/

std::shared_ptr<s2::Joint> Symbol::JointLoader::Create(const s2::SprPtr& spr, const s2::JointPose& joint_pose) const
{
	return std::make_shared<Joint>(spr, joint_pose);
}

}