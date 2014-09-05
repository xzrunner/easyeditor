#include "EditUVOP.h"
#include "StagePanel.h"
#include "Shape.h"

namespace emesh
{

EditUVOP::EditUVOP(StagePanel* stage)
	: d2d::ZoomViewOP(stage, true)
	, m_stage(stage)
{
}

bool EditUVOP::onDraw(const d2d::Screen& scr) const
{
	if (Shape* shape = m_stage->getShape())
	{
		shape->DrawTexture(scr, d2d::Matrix());
		shape->DrawInfoXY(scr);
	}

	return false;
}

}