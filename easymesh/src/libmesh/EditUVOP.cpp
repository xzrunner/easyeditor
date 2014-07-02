#include "EditUVOP.h"
#include "StagePanel.h"
#include "Mesh.h"

namespace emesh
{

EditUVOP::EditUVOP(StagePanel* stage)
	: d2d::ZoomViewOP(stage, true)
	, m_stage(stage)
{
}

bool EditUVOP::onDraw(const d2d::Screen& scr) const
{
	if (Mesh* mesh = m_stage->getMesh())
	{
		mesh->drawTexture(scr);
		mesh->drawInfoXY(scr);
	}

	return false;
}

}