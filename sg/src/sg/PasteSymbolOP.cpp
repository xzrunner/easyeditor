#include "PasteSymbolOP.h"
#include "StagePanel.h"
#include "SymbolInfo.h"

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

bool PasteSymbolOP::onDraw() const
{
	if (isCurrSymbolValid()) {
		return d2d::PasteSymbolOP::onDraw();
	} else {
		return false;
	}
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

}