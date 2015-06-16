#include "CreateMeshOP.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "Shape.h"

namespace emesh
{

CreateMeshOP::CreateMeshOP(StagePanel* stage)
	: libshape::EditPolylineOP<libshape::DrawLoopOP, libshape::SelectNodesOP>(stage, stage, NULL, new d2d::OneFloatValueStatic(5), NULL)
	, m_stage(stage)
{
	SetLoop(true);
}

bool CreateMeshOP::OnKeyDown(int keyCode)
{
	if (libshape::EditPolylineOP<libshape::DrawLoopOP, libshape::SelectNodesOP>::OnKeyDown(keyCode))
		return true;

	switch (keyCode)
	{
	case 'a': case 'A':
	case 'd': case 'D':
	case 's': case 'S':
	case 'w': case 'W':
		if (IsDirty()) {
			m_stage->UpdateSymbol();
			ResetDirty();
		}
		break;
	}

	return false;
}

bool CreateMeshOP::OnMouseLeftDown(int x, int y)
{
	if (libshape::EditPolylineOP<libshape::DrawLoopOP, libshape::SelectNodesOP>::OnMouseLeftDown(x, y))
		return true;

	if (IsDirty()) {
		m_stage->UpdateSymbol();
		ResetDirty();
	}

	return false;
}

bool CreateMeshOP::OnMouseLeftUp(int x, int y)
{
	if (libshape::EditPolylineOP<libshape::DrawLoopOP, libshape::SelectNodesOP>::OnMouseLeftUp(x, y))
		return true;

	if (IsDirty()) {
		m_stage->UpdateSymbol();
		ResetDirty();
	}

	return false;
}

bool CreateMeshOP::OnMouseRightDown(int x, int y)
{
	if (libshape::EditPolylineOP<libshape::DrawLoopOP, libshape::SelectNodesOP>::OnMouseRightDown(x, y))
		return true;

	if (IsDirty()) {
		m_stage->UpdateSymbol();
		ResetDirty();
	}

	return false;
}

bool CreateMeshOP::OnMouseRightUp(int x, int y)
{
	if (libshape::EditPolylineOP<libshape::DrawLoopOP, libshape::SelectNodesOP>::OnMouseRightUp(x, y))
		return true;

	if (IsDirty()) {
		m_stage->UpdateSymbol();
		ResetDirty();
	}

	return false;
}

bool CreateMeshOP::OnDraw() const
{
	if (d2d::ZoomViewOP::OnDraw())
		return true;

	if (const d2d::Image* image = m_stage->GetSymbol()->getImage())
 	{
		d2d::Matrix mt;
		image->draw(mt, image->getRegion());
 	}

	if (Shape* shape = m_stage->GetShape())
	{
		shape->DrawInfoUV();
	}

	libshape::EditPolylineOP<libshape::DrawLoopOP, libshape::SelectNodesOP>::OnDraw();

	return false;
}

}