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

bool EditUVOP::OnDraw() const
{
	if (Shape* shape = m_stage->GetShape())
	{
		shape->DrawTexture(d2d::Matrix());
		shape->DrawInfoXY();
	}

	return false;
}

}