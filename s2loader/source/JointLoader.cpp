#include "s2loader/JointLoader.h"

#include <sprite2/Joint.h>

namespace s2loader
{

std::shared_ptr<s2::Joint> JointLoader::Create(const s2::SprPtr& spr, const s2::JointPose& joint_pose) const
{
	return std::make_shared<s2::Joint>(spr, joint_pose);
}

}