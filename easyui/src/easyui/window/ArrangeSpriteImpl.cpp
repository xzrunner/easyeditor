#include "ArrangeSpriteImpl.h"
#include "TranslateSpriteState.h"
#include "StagePanel.h"

namespace eui
{
namespace window
{

ArrangeSpriteImpl::ArrangeSpriteImpl(StagePanel* stage, ee::PropertySettingPanel* property)
	: ee::ArrangeSpriteImpl(stage, stage->GetStageImpl(), stage, property)
	, m_stage(stage)
{
}

ee::ArrangeSpriteState* ArrangeSpriteImpl::
CreateTranslateState(ee::SpriteSelection* selection, const ee::Vector& first_pos) const
{
	return new TranslateSpriteState(m_stage, selection, first_pos, m_stage->GetAnchorMgr()); 
}

}
}