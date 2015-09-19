#include "EditChangedRectOP.h"
#include "StagePanel.h"
#include "ChangedRectIcon.h"
#include "IconType.h"

namespace eicon
{

EditChangedRectOP::EditChangedRectOP(StagePanel* stage)
	: d2d::ZoomViewOP(stage, stage->GetStageImpl(), true)
	, m_stage(stage)
{
}

bool EditChangedRectOP::OnActive()
{
	if (d2d::ZoomViewOP::OnActive()) {
		return true;
	}

	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	Icon* icon = stage->GetIcon();
	if (icon && get_icon_type(icon->GetIconDesc()) == IT_CHANGED_RECT) {
		return false;
	}

	icon = new ChangedRectIcon;
	stage->SetIcon(icon);
	icon->Release();	

	return false;
}

}