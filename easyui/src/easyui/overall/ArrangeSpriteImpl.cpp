#include "ArrangeSpriteImpl.h"
#include "TranslateSpriteState.h"
#include "StagePanel.h"

namespace eui
{
namespace overall
{

ArrangeSpriteImpl::ArrangeSpriteImpl(StagePanel* stage, d2d::PropertySettingPanel* property)
	: d2d::ArrangeSpriteImpl(stage, stage->GetStageImpl(), stage, property, NULL)
	, m_stage(stage)
{
}

d2d::IArrangeSpriteState* ArrangeSpriteImpl::
CreateTransalteState(d2d::SpriteSelection* selection, const d2d::Vector& first_pos) const
{
	return new TranslateSpriteState(selection, first_pos, m_stage->GetAnchorMgr()); 
}

}
}