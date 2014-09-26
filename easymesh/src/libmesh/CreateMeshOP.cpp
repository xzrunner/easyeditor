#include "CreateMeshOP.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "Shape.h"

namespace emesh
{

CreateMeshOP::CreateMeshOP(StagePanel* stage)
	: libshape::EditPolylineOP<libshape::DrawLoopOP, libshape::SelectNodesOP>(stage, stage, NULL, NULL, 5)
	, m_stage(stage)
{
	SetLoop(true);
}

bool CreateMeshOP::onKeyDown(int keyCode)
{
	if (libshape::EditPolylineOP<libshape::DrawLoopOP, libshape::SelectNodesOP>::onKeyDown(keyCode))
		return true;

	switch (keyCode)
	{
	case 'a': case 'A':
	case 'd': case 'D':
	case 's': case 'S':
	case 'w': case 'W':
		m_stage->UpdateSymbol();
		break;
	}

	return false;
}

bool CreateMeshOP::onMouseLeftDown(int x, int y)
{
	if (libshape::EditPolylineOP<libshape::DrawLoopOP, libshape::SelectNodesOP>::onMouseLeftDown(x, y))
		return true;

	m_stage->UpdateSymbol();

	return false;
}

bool CreateMeshOP::onMouseLeftUp(int x, int y)
{
	if (libshape::EditPolylineOP<libshape::DrawLoopOP, libshape::SelectNodesOP>::onMouseLeftUp(x, y))
		return true;

	m_stage->UpdateSymbol();

	return false;
}

bool CreateMeshOP::onMouseRightDown(int x, int y)
{
	if (libshape::EditPolylineOP<libshape::DrawLoopOP, libshape::SelectNodesOP>::onMouseRightDown(x, y))
		return true;

	m_stage->UpdateSymbol();

	return false;
}

bool CreateMeshOP::onMouseRightUp(int x, int y)
{
	if (libshape::EditPolylineOP<libshape::DrawLoopOP, libshape::SelectNodesOP>::onMouseRightUp(x, y))
		return true;

	m_stage->UpdateSymbol();

	return false;
}

bool CreateMeshOP::onDraw(const d2d::Screen& scr) const
{
 	if (const d2d::Image* image = m_stage->GetSymbol()->getImage())
  	{
 		d2d::Matrix mt;
 		image->draw(scr, mt, image->getRegion());
  	}

	if (Shape* shape = m_stage->GetShape())
	{
		shape->DrawInfoUV(scr);
	}

	libshape::EditPolylineOP<libshape::DrawLoopOP, libshape::SelectNodesOP>::onDraw(scr);

	return false;
}

}