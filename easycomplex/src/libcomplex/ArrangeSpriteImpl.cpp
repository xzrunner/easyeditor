#include "ArrangeSpriteImpl.h"
#include "StagePanel.h"
#include "SphereRotateState.h"

#include <easyparticle3d.h>

namespace ecomplex
{

ArrangeSpriteImpl::ArrangeSpriteImpl(StagePanel* stage, ee::PropertySettingPanel* property)
	: ee::ArrangeSpriteImpl(stage, stage->GetStageImpl(), stage, property)
{
}

ee::ArrangeSpriteState* ArrangeSpriteImpl::CreateRotateState(ee::SpriteSelection* selection, 
															   const ee::Vector& first_pos) const
{
	std::vector<ee::Sprite*> sprites;
	selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));
	if (sprites.size() == 1 && dynamic_cast<eparticle3d::Sprite*>(sprites[0])) {
		return new SphereRotateState(m_stage, first_pos, static_cast<eparticle3d::Sprite*>(sprites[0])->GetDir());
	} else {
		return ee::ArrangeSpriteImpl::CreateRotateState(selection, first_pos);
	}
}

}