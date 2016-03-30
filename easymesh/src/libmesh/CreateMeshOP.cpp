#include "CreateMeshOP.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "Shape.h"

#include <ee/Matrix.h>
#include <ee/Image.h>
#include <ee/RenderParams.h>

namespace emesh
{

CreateMeshOP::CreateMeshOP(StagePanel* stage)
	: eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>(stage, stage->GetStageImpl(), stage, NULL, new ee::OneFloatValueStatic(5), NULL)
	, m_stage(stage)
{
	SetLoop(true);
}

bool CreateMeshOP::OnKeyDown(int keyCode)
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnKeyDown(keyCode))
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
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseLeftDown(x, y))
		return true;

	if (IsDirty()) {
		m_stage->UpdateSymbol();
		ResetDirty();
	}

	return false;
}

bool CreateMeshOP::OnMouseLeftUp(int x, int y)
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseLeftUp(x, y))
		return true;

	if (IsDirty()) {
		m_stage->UpdateSymbol();
		ResetDirty();
	}

	return false;
}

bool CreateMeshOP::OnMouseRightDown(int x, int y)
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseRightDown(x, y))
		return true;

	if (IsDirty()) {
		m_stage->UpdateSymbol();
		ResetDirty();
	}

	return false;
}

bool CreateMeshOP::OnMouseRightUp(int x, int y)
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseRightUp(x, y))
		return true;

	if (IsDirty()) {
		m_stage->UpdateSymbol();
		ResetDirty();
	}

	return false;
}

bool CreateMeshOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw())
		return true;

	if (const ee::Image* image = m_stage->GetSymbol()->getImage())
 	{
		image->Draw(ee::RenderParams());
 	}

	if (Shape* shape = m_stage->GetShape())
	{
		shape->DrawInfoUV();
	}

	eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnDraw();

	return false;
}

}