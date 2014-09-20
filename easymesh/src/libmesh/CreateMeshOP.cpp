#include "CreateMeshOP.h"
#include "StagePanel.h"
#include "Shape.h"
#include "Triangle.h"
#include "Sprite.h"

namespace emesh
{

CreateMeshOP::CreateMeshOP(StagePanel* stage)
	: libshape::EditPolylineOP<libshape::DrawPenLineOP, d2d::SelectShapesOP>(stage, stage, NULL, NULL, 5)
	, m_stage(stage)
	, m_selected(NULL)
{
	SetLoop(true);
	m_last_right.setInvalid();
}

bool CreateMeshOP::onMouseLeftDown(int x, int y)
{
	if (libshape::EditPolylineOP<libshape::DrawPenLineOP, d2d::SelectShapesOP>::onMouseLeftDown(x, y))
		return true;

	if (Shape* shape = m_stage->getShape())
	{
		d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
		m_selected = shape->Find(pos);
	}

	return false;
}

bool CreateMeshOP::onMouseLeftUp(int x, int y)
{
	if (libshape::EditPolylineOP<libshape::DrawPenLineOP, d2d::SelectShapesOP>::onMouseLeftUp(x, y))
		return true;

	Shape* shape = m_stage->getShape();
	if (!shape) return false;

	if (!m_selected)
	{
		d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
		shape->Insert(pos);
		m_editPanel->Refresh();
	}

	return false;
}

bool CreateMeshOP::onMouseRightDown(int x, int y)
{
	if (libshape::EditPolylineOP<libshape::DrawPenLineOP, d2d::SelectShapesOP>::onMouseRightDown(x, y))
		return true;

	if (Shape* shape = m_stage->getShape())
	{
		d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
		shape->Remove(pos);
		m_editPanel->Refresh();

		m_last_right = pos;
	}

	return false;
}

bool CreateMeshOP::onMouseRightUp(int x, int y)
{
	if (libshape::EditPolylineOP<libshape::DrawPenLineOP, d2d::SelectShapesOP>::onMouseRightUp(x, y))
		return true;

	m_last_right.setInvalid();

	return false;
}

bool CreateMeshOP::onMouseDrag(int x, int y)
{
	if (libshape::EditPolylineOP<libshape::DrawPenLineOP, d2d::SelectShapesOP>::onMouseDrag(x, y))
		return true;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);

	// move background
	if (m_last_right.isValid())
	{
		d2d::Vector offset = pos - m_last_right;
		StagePanel* stage = static_cast<StagePanel*>(m_stage);
		stage->TranslateBackground(offset);
		m_last_right = pos;
	}

	Shape* shape = m_stage->getShape();
	if (!shape) return false;

	if (m_selected) {
		shape->Move(m_selected, pos);
	}

	m_editPanel->Refresh();

	return false;
}

bool CreateMeshOP::onDraw(const d2d::Screen& scr) const
{
	if (libshape::EditPolylineOP<libshape::DrawPenLineOP, d2d::SelectShapesOP>::onDraw(scr))
		return true;

	if (const d2d::Image* image = m_stage->getSprite()->getImage())
 	{
		d2d::Matrix mt;
		image->draw(scr, mt, image->getRegion());
 	}

	if (Shape* shape = m_stage->getShape())
	{
		shape->DrawInfoUV(scr);
	}

	return false;
}

}