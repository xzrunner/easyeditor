#include "CreateMeshOP.h"
#include "StagePanel.h"
#include "Mesh.h"
#include "Triangle.h"
#include "Sprite.h"

namespace emesh
{

CreateMeshOP::CreateMeshOP(StagePanel* stage)
	: d2d::ZoomViewOP(stage, true, false)
	, m_stage(stage)
	, m_selected(NULL)
{
}

bool CreateMeshOP::onMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftDown(x, y))
		return true;

	if (Mesh* mesh = m_stage->getMesh())
	{
		d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
		m_selected = mesh->find(pos);
	}

	return false;
}

bool CreateMeshOP::onMouseLeftUp(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftUp(x, y))
		return true;

	Mesh* mesh = m_stage->getMesh();
	if (!mesh) return false;

	if (!m_selected)
	{
		d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
		mesh->insert(pos);
	}

	return false;
}

bool CreateMeshOP::onMouseRightDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseRightDown(x, y))
		return true;

	if (Mesh* mesh = m_stage->getMesh())
	{
		d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
		mesh->remove(pos);
	}

	return false;
}

bool CreateMeshOP::onMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseDrag(x, y))
		return true;

	Mesh* mesh = m_stage->getMesh();
	if (!mesh) return false;

	if (m_selected)
	{
		d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
		mesh->move(m_selected, pos);
	}

	return false;
}

bool CreateMeshOP::onDraw() const
{
	if (d2d::ZoomViewOP::onDraw())
		return true;

	d2d::ISprite* sprite = m_stage->getSprite();
 	if (sprite)
 	{
 		d2d::SpriteDraw::drawSprite(sprite);
 	}

	if (Mesh* mesh = m_stage->getMesh())
	{
		mesh->drawInfo();
	}

	return false;
}

}