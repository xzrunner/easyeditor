#include "PasteSymbolOP.h"
#include "StagePanel.h"
#include "SymbolInfo.h"

namespace sg
{

PasteSymbolOP::PasteSymbolOP(StagePanel* stage, d2d::LibraryPanel* library)
	: d2d::PasteSymbolOP(stage, stage, library)
	, m_render(stage)
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

	m_render.DrawGrass(*symbol, m_pos);

	StagePanel* stage = static_cast<StagePanel*>(m_editPanel);
	bool valid = stage->getCheckBoard().IsValid(*symbol, m_pos);
	m_render.DrawGrids(*symbol, m_pos, valid);

	bool ret = d2d::PasteSymbolOP::onDraw();

	m_render.DrawArrow(*symbol, m_pos);

	return ret;
}

bool PasteSymbolOP::isCurrSymbolValid() const
{
	d2d::ISymbol* symbol = m_libraryPanel->getSymbol();
	if (!symbol) {
		return false;
	}

	SymbolInfo* info = static_cast<SymbolInfo*>(symbol->getUserData());
	if (info == NULL) {
		return false;
	}

	return info->remain > 0;
}

bool PasteSymbolOP::isCurrSymbolIsWall() const
{
	d2d::ISymbol* symbol = m_libraryPanel->getSymbol();
	if (!symbol) {
		return false;
	}

	SymbolInfo* info = static_cast<SymbolInfo*>(symbol->getUserData());
	if (info == NULL) {
		return false;
	}

	return info->iswall;	
}

}