#include "PasteSymbolOP.h"
#include "StagePanel.h"
#include "SymbolExt.h"
#include "SymbolRender.h"
#include "tools.h"

#include <ee/LibraryPanel.h>

namespace sg
{

PasteSymbolOP::PasteSymbolOP(StagePanel* stage, ee::LibraryPanel* library)
	: ee::PasteSymbolOP(stage, stage->GetStageImpl(), library)
{
}

bool PasteSymbolOP::OnMouseLeftDown(int x, int y)
{
	if (isCurrSymbolValid()) {
		return ee::PasteSymbolOP::OnMouseLeftDown(x, y);
	} else {
		return false;
	}
}

bool PasteSymbolOP::OnMouseDrag(int x, int y)
{
	if (isCurrSymbolValid() && isCurrSymbolIsWall()) {
		return ee::PasteSymbolOP::OnMouseLeftDown(x, y);
	} else {
		return false;
	}
}

bool PasteSymbolOP::OnDraw() const
{
	if (!isCurrSymbolValid()) {
		return false;
	}

	bool ret;

	ee::Symbol* sym = m_library->GetSymbol();

	SymbolRender* render = SymbolRender::Instance();
	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	bool is_flat = stage->GetPerspective();

	render->DrawGrass(*sym, m_pos, is_flat);

	bool valid = stage->GetCheckBoard().IsValid(*sym, m_pos);
	render->DrawGrids(*sym, m_pos, valid, is_flat);

	ret = ee::PasteSymbolOP::OnDraw();

	if (!is_flat) {
		render->DrawArrow(*sym, m_pos);
	}

	return ret;
}

bool PasteSymbolOP::isCurrSymbolValid() const
{
// 	ee::Symbol* sym = m_library->getSymbol();
// 	if (!sym) {
// 		return false;
// 	}
// 
// 	SymbolExt* info = static_cast<SymbolExt*>(sym->GetUserData());
// 	if (info == NULL) {
// 		return false;
// 	}
// 
// 	return info->remain > 0;

	//////////////////////////////////////////////////////////////////////////

	return true;
}

bool PasteSymbolOP::isCurrSymbolIsWall() const
{
	ee::Symbol* sym = m_library->GetSymbol();
	return IsSymbolWall(*sym);
}

}