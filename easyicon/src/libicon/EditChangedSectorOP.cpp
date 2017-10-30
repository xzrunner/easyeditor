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
	Icon* old_icon = stage->GetIcon();
	if (old_icon && get_icon_type(old_icon->GetIconDesc()) == IT_CHANGED_SECTOR) {
		return false;
	}

	std::unique_ptr<s2::Icon> new_icon = std::make_unique<ChangedSectorIcon>();
	stage->SetIcon(new_icon);

	return false;
}

}