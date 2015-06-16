#include "EditChangedRectOP.h"
#include "StagePanel.h"
#include "ChangedRectIcon.h"

namespace eicon
{

EditChangedRectOP::EditChangedRectOP(StagePanel* stage)
	: d2d::ZoomViewOP(stage, true)
	, m_stage(stage)
{
}

bool EditChangedRectOP::OnActive()
{
	if (d2d::ZoomViewOP::OnActive()) {
		return true;
	}

	ChangedRectIcon* icon = new ChangedRectIcon;
	m_stage->SetIcon(icon);
	icon->Release();

	return false;
}

}