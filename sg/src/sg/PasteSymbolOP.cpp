#include "PasteSymbolOP.h"
#include "StagePanel.h"
#include "SymbolExt.h"
#include "SymbolRender.h"
#include "tools.h"

namespace sg
{

PasteSymbolOP::PasteSymbolOP(StagePanel* stage, d2d::LibraryPanel* library)
	: d2d::PasteSymbolOP(stage, stage->GetStageImpl(), stage, library)
{
}

bool PasteSymbolOP::OnMouseLeftDown(int x, int y)
{
	if (isCurrSymbolValid()) {
		return d2d::PasteSymbolOP::OnMouseLeftDown(x, y);
	} else {
		return false;
	}
}

bool PasteSymbolOP::OnMouseDrag(int x, int y)
{
	if (isCurrSymbolValid() && isCurrSymbolIsWall()) {
		return d2d::PasteSymbolOP::OnMouseLeftDown(x, y);
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

	d2d::ISymbol* symbol = m_libraryPanel->GetSymbol();

	SymbolRender* render = SymbolRender::Instance();
	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	bool is_flat = stage->GetPerspective();

	render->DrawGrass(*symbol, m_pos, is_flat);

	bool valid = stage->GetCheckBoard().IsValid(*symbol, m_pos);
	render->DrawGrids(*symbol, m_pos, valid, is_flat);

	ret = d2d::PasteSymbolOP::OnDraw();

	if (!is_flat) {
		render->DrawArrow(*symbol, m_pos);
	}

	return ret;
}

bool PasteSymbolOP::isCurrSymbolValid() const
{
// 	d2d::ISymbol* symbol = m_libraryPanel->getSymbol();
// 	if (!symbol) {
// 		return false;
// 	}
// 
// 	SymbolExt* info = static_cast<SymbolExt*>(symbol->GetUserData());
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
	d2d::ISymbol* symbol = m_libraryPanel->GetSymbol();
	return IsSymbolWall(*symbol);
}

}