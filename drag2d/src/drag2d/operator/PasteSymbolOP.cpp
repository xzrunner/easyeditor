#include "PasteSymbolOP.h"

#include "dataset/SpriteFactory.h"
#include "view/LibraryPanel.h"
#include "view/MultiSpritesImpl.h"
#include "view/GLCanvas.h"
#include "render/SpriteDraw.h"

namespace d2d
{

PasteSymbolOP::PasteSymbolOP(EditPanel* editPanel, MultiSpritesImpl* panelImpl, 
							 LibraryPanel* libraryPanel, float* pScale/* = NULL*/)
	: ZoomViewOP(editPanel, true)
	, m_panelImpl(panelImpl)
	, m_libraryPanel(libraryPanel)
	, m_pScale(pScale)
{
	m_pos.setInvalid();
}

bool PasteSymbolOP::onMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::onMouseLeftDown(x, y)) return true;

	ISymbol* symbol = m_libraryPanel->getSymbol();
	if (symbol)
	{
		m_pos = m_editPanel->transPosScreenToProject(x, y);
		ISprite* sprite = SpriteFactory::Instance()->create(symbol);
		sprite->translate(m_pos);
		if (m_pScale)
			sprite->setScale(*m_pScale, *m_pScale);
		m_panelImpl->insertSprite(sprite);
		sprite->release();
	}

	return false;
}

bool PasteSymbolOP::onMouseMove(int x, int y)
{
	if (ZoomViewOP::onMouseMove(x, y)) return true;

	m_pos = m_editPanel->transPosScreenToProject(x, y);
	m_editPanel->Refresh();

	return false;
}

bool PasteSymbolOP::onDraw(const Screen& scr) const
{
	if (ZoomViewOP::onDraw(scr)) return true;

	ISymbol* symbol = m_libraryPanel->getSymbol();
	if (symbol && m_pos.isValid())
	{
		if (m_pScale)
			SpriteDraw::drawSprite(scr, symbol, Matrix(), m_pos, 0.0f, *m_pScale);
		else
			SpriteDraw::drawSprite(scr, symbol, Matrix(), m_pos);
	}

	return false;
}

bool PasteSymbolOP::clear()
{
	if (ZoomViewOP::clear()) return true;

	m_pos.setInvalid();

	return false;
}

} // d2d