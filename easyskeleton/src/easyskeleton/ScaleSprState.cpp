#include "ScaleSprState.h"
#include "Bone.h"

#include <ee/Sprite.h>

namespace eskeleton
{

ScaleSprState::ScaleSprState(ee::Sprite* spr, const ee::SpriteCtrlNode::Node& ctrl_node)
	: ee::ScaleSpriteState(spr, ctrl_node)
{
}

void ScaleSprState::SetScaleTimes(const sm::vec2& st)
{
	if (!m_spr) {
		return;
	}

	Bone* bone = (Bone*)(m_spr->GetUserData());
	bone->Scale(st);
}

}