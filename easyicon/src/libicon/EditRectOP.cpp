#include "EditRectOP.h"
#include "StagePanel.h"
#include "RectIcon.h"

namespace eicon
{

EditRectOP::EditRectOP(StagePanel* stage)
	: d2d::ZoomViewOP(stage, true)
	, m_stage(stage)
{
}

bool EditRectOP::onActive()
{
	if (d2d::ZoomViewOP::onActive()) {
		return true;
	}

	RectIcon* icon = new RectIcon;
	m_stage->SetIcon(icon);
	icon->Release();

	return false;
}

}