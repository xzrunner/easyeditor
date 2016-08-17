#include "ArrangeSpriteImpl.h"
#include "StagePanel.h"
#include "SphereRotateState.h"

#include <ee/SpriteSelection.h>
#include <ee/FetchAllVisitor.h>

#include <easyparticle3d.h>

namespace ecomplex
{

ArrangeSpriteImpl::ArrangeSpriteImpl(StagePanel* stage, ee::PropertySettingPanel* property)
	: ee::ArrangeSpriteImpl(stage, stage->GetStageImpl(), stage, property)
{
}

ee::ArrangeSpriteState* ArrangeSpriteImpl::CreateRotateState(ee::SpriteSelection* selection, 
															 const sm::vec2& first_pos) const
{
	std::vector<ee::Sprite*> sprs;
	selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprs));
	if (sprs.size() == 1 && dynamic_cast<eparticle3d::Sprite*>(sprs[0])) {
		return new SphereRotateState(m_stage, first_pos, static_cast<eparticle3d::Sprite*>(sprs[0])->GetDir());
	} else {
		return ee::ArrangeSpriteImpl::CreateRotateState(selection, first_pos);
	}
}

}