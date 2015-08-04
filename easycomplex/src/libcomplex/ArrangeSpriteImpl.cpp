#include "ArrangeSpriteImpl.h"
#include "StagePanel.h"
#include "SphereRotateState.h"

#include <easyparticle3d.h>

namespace ecomplex
{

ArrangeSpriteImpl::ArrangeSpriteImpl(StagePanel* stage, d2d::PropertySettingPanel* property)
	: d2d::ArrangeSpriteImpl(stage, stage, property)
{
}

d2d::IArrangeSpriteState* ArrangeSpriteImpl::CreateRotateState(d2d::SpriteSelection* selection, 
															   const d2d::Vector& first_pos) const
{
	std::vector<d2d::ISprite*> sprites;
	selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	if (sprites.size() == 0 && dynamic_cast<eparticle3d::Sprite*>(sprites[0])) {
		return new SphereRotateState(m_stage, static_cast<eparticle3d::Sprite*>(sprites[0])->GetDir());
	} else {
		return d2d::ArrangeSpriteImpl::CreateRotateState(selection, first_pos);
	}
}

}