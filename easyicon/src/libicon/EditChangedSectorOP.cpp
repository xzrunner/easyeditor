#include "EditChangedSectorOP.h"
#include "StagePanel.h"
#include "ChangedSectorIcon.h"

namespace eicon
{

EditChangedSectorOP::EditChangedSectorOP(StagePanel* stage)
	: d2d::ZoomViewOP(stage, true)
	, m_stage(stage)
{
}

bool EditChangedSectorOP::onActive()
{
	if (d2d::ZoomViewOP::onActive()) {
		return true;
	}

	ChangedSectorIcon* icon = new ChangedSectorIcon;
	m_stage->SetIcon(icon);
	icon->Release();

	return false;
}

}