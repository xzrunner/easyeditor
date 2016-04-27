#include "CreateNWOP.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "Mesh.h"

#include <ee/Matrix.h>
#include <ee/Image.h>
#include <ee/RenderParams.h>

namespace emesh
{

CreateNWOP::CreateNWOP(StagePanel* stage)
	: eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>(stage, stage->GetStageImpl(), stage, NULL, new ee::OneFloatValueStatic(5), NULL)
	, m_stage(stage)
{
	SetLoop(true);
}

bool CreateNWOP::OnKeyDown(int keyCode)
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

bool CreateNWOP::OnMouseLeftDown(int x, int y)
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseLeftDown(x, y))
		return true;

	if (IsDirty()) {
		m_stage->UpdateSymbol();
		ResetDirty();
	}

	return false;
}

bool CreateNWOP::OnMouseLeftUp(int x, int y)
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseLeftUp(x, y))
		return true;

	if (IsDirty()) {
		m_stage->UpdateSymbol();
		ResetDirty();
	}

	return false;
}

bool CreateNWOP::OnMouseRightDown(int x, int y)
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseRightDown(x, y))
		return true;

	if (IsDirty()) {
		m_stage->UpdateSymbol();
		ResetDirty();
	}

	return false;
}

bool CreateNWOP::OnMouseRightUp(int x, int y)
{
	if (eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnMouseRightUp(x, y))
		return true;

	if (IsDirty()) {
		m_stage->UpdateSymbol();
		ResetDirty();
	}

	return false;
}

bool CreateNWOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw())
		return true;

	if (const ee::Image* image = m_stage->GetSymbol()->getImage())
 	{
		image->Draw(ee::RenderParams());
 	}

	if (Mesh* mesh = m_stage->GetMesh())
	{
		mesh->DrawInfoUV();
	}

	eshape::EditPolylineOP<eshape::DrawLoopOP, eshape::SelectNodesOP>::OnDraw();

	return false;
}

}