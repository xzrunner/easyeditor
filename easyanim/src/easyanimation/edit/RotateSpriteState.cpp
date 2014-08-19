#include "RotateSpriteState.h"

#include "dataset/SkeletonData.h"
#include "view/StagePanel.h"

namespace eanim
{

RotateSpriteState::RotateSpriteState(StagePanel* stage, 
									 d2d::SpriteSelection* selection, 
									 const d2d::Vector& first_pos)
	: d2d::RotateSpriteState(selection, first_pos)
	, m_stage(stage)
{
}

void RotateSpriteState::Rotate(const d2d::Vector& dst)
{
	d2d::RotateSpriteState::Rotate(dst);

	d2d::SpriteSelection* selection = GetSelection();
	if (!selection->empty())
	{
		std::vector<d2d::ISprite*> sprites;
		selection->traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
		m_stage->getSkeletonData().fixJoint(sprites[0]);
		float dAngle = d2d::Math::getAngleInDirection(sprites[0]->getPosition(), GetLastPos(), dst);
		m_stage->getSkeletonData().updateJoint(sprites[0], dAngle);
	}
}

}