#include "EditMeshOP.h"
#include "StagePanel.h"
#include "Mesh.h"
#include "Triangle.h"

namespace emesh
{

EditMeshOP::EditMeshOP(StagePanel* stage)
	: d2d::ZoomViewOP(stage, true, false)
	, m_stage(stage)
{
}

bool EditMeshOP::onMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftDown(x, y))
		return true;

	if (Mesh* mesh = m_stage->getMesh())
	{
		d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
		m_selected = mesh->queryNode(pos);
	}

	return false;
}

bool EditMeshOP::onMouseLeftUp(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftUp(x, y))
		return true;

	return false;
}

bool EditMeshOP::onMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseDrag(x, y))
		return true;

	Mesh* mesh = m_stage->getMesh();
	if (!mesh) return false;

	if (m_selected)
	{
		d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
		m_selected->xy = pos;
	}

	return false;
}

bool EditMeshOP::onDraw() const
{
	if (d2d::ZoomViewOP::onDraw())
		return true;

	if (Mesh* mesh = m_stage->getMesh())
	{
		mesh->drawTexture();
		mesh->drawInfo();
	}

	return false;
}

}