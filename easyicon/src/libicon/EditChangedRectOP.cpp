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

bool EditChangedRectOP::onActive()
{
	if (d2d::ZoomViewOP::onActive()) {
		return true;
	}

	ChangedRectIcon* icon = new ChangedRectIcon;
	icon->SetProcess(0.15f);
	m_stage->SetIcon(icon);
	icon->Release();

	return false;
}

}