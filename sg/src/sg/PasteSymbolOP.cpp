#include "PasteSymbolOP.h"
#include "StagePanel.h"
#include "SymbolExt.h"
#include "SymbolRender.h"
#include "tools.h"

namespace sg
{

PasteSymbolOP::PasteSymbolOP(StagePanel* stage, d2d::LibraryPanel* library)
	: d2d::PasteSymbolOP(stage, stage, library)
{
}

bool PasteSymbolOP::onMouseLeftDown(int x, int y)
{
	if (isCurrSymbolValid()) {
		return d2d::PasteSymbolOP::onMouseLeftDown(x, y);
	} else {
		return false;
	}
}

bool PasteSymbolOP::onMouseDrag(int x, int y)
{
	if (isCurrSymbolValid() && isCurrSymbolIsWall()) {
		return d2d::PasteSymbolOP::onMouseLeftDown(x, y);
	} else {
		return false;
	}
}

bool PasteSymbolOP::onDraw() const
{
	if (!isCurrSymbolValid()) {
		return false;
	}

	d2d::ISymbol* symbol = m_libraryPanel->getSymbol();

	SymbolRender* render = SymbolRender::Instance();
	render->DrawGrass(*symbol, m_pos);

	StagePanel* stage = static_cast<StagePanel*>(m_editPanel);
	bool valid = stage->GetCheckBoard().IsValid(*symbol, m_pos);
	render->DrawGrids(*symbol, m_pos, valid);

	bool ret = d2d::PasteSymbolOP::onDraw();

	render->DrawArrow(*symbol, m_pos);

	return ret;
}

bool PasteSymbolOP::isCurrSymbolValid() const
{
	d2d::ISymbol* symbol = m_libraryPanel->getSymbol();
	if (!symbol) {
		return false;
	}

	SymbolExt* info = static_cast<SymbolExt*>(symbol->getUserData());
	if (info == NULL) {
		return false;
	}

	return info->remain > 0;
}

bool PasteSymbolOP::isCurrSymbolIsWall() const
{
	d2d::ISymbol* symbol = m_libraryPanel->getSymbol();
	return IsSymbolWall(*symbol);
}

}