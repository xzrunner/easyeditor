#include "PasteSymbolCaptureOP.h"

#include "common/Math.h"
#include "dataset/SpriteFactory.h"
#include "view/MultiSpritesImpl.h"
#include "view/LibraryPanel.h"

namespace d2d
{

PasteSymbolCaptureOP::PasteSymbolCaptureOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl,
										   LibraryPanel* libraryPanel, PasteSymbolOffsetCMPT<PasteSymbolCaptureOP>* cmpt)
	: PasteSymbolOP(editPanel, spritesImpl, libraryPanel)
	, m_cmpt(cmpt)
	, m_bCaptured(false)
{
	m_lastPos.setInvalid();
}

bool PasteSymbolCaptureOP::onMouseLeftDown(int x, int y)
{
	ISymbol* symbol = m_libraryPanel->getSymbol();
	if (symbol) 
	{
		if (!m_bCaptured)
			m_pos = m_editPanel->transPosScreenToProject(x, y);
		m_lastPos = m_pos;

		ISprite* sprite = SpriteFactory::Instance()->create(symbol);
		sprite->translate(m_pos);
		m_panelImpl->insertSprite(sprite);
		sprite->release();
	}

	return false;
}

bool PasteSymbolCaptureOP::onMouseMove(int x, int y)
{
	if (ZoomViewOP::onMouseMove(x, y)) return true;

	m_bCaptured = false;
	m_pos = m_editPanel->transPosScreenToProject(x, y);
	if (m_lastPos.isValid())
	{
		Vector offset = m_cmpt->getOffset();
		Vector newPos = m_lastPos + offset;
		if (Math::getDistance(m_pos, newPos) < 
			Math::getDistance(offset, Vector(0, 0)))
		{
			m_bCaptured = true;
			m_pos = newPos;
		}
	}
	m_editPanel->Refresh();

	return false;
}

bool PasteSymbolCaptureOP::clear()
{
	if (PasteSymbolOP::clear()) return true;

	m_lastPos.setInvalid();

	return false;
}

} // d2d