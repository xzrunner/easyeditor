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

	ee::Symbol* symbol = m_library->GetSymbol();

	SymbolRender* render = SymbolRender::Instance();
	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	bool is_flat = stage->GetPerspective();

	render->DrawGrass(*symbol, m_pos, is_flat);

	bool valid = stage->GetCheckBoard().IsValid(*symbol, m_pos);
	render->DrawGrids(*symbol, m_pos, valid, is_flat);

	ret = ee::PasteSymbolOP::OnDraw();

	if (!is_flat) {
		render->DrawArrow(*symbol, m_pos);
	}

	return ret;
}

bool PasteSymbolOP::isCurrSymbolValid() const
{
// 	ee::Symbol* symbol = m_library->getSymbol();
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
	ee::Symbol* symbol = m_library->GetSymbol();
	return IsSymbolWall(*symbol);
}

}