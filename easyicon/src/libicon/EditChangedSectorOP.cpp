#include "EditChangedSectorOP.h"
#include "StagePanel.h"
#include "ChangedSectorIcon.h"
#include "IconType.h"

namespace eicon
{

EditChangedSectorOP::EditChangedSectorOP(StagePanel* stage)
	: ee::ZoomViewOP(stage, stage->GetStageImpl(), true)
	, m_stage(stage)
{
}

bool EditChangedSectorOP::OnActive()
{
	if (ee::ZoomViewOP::OnActive()) {
		return true;
	}

	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	Icon* icon = stage->GetIcon();
	if (icon && get_icon_type(icon->GetIconDesc()) == IT_CHANGED_SECTOR) {
		return false;
	}

	icon = new ChangedSectorIcon;
	stage->SetIcon(icon);
	icon->RemoveReference();	

	return false;
}

}