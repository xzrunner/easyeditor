#include "PasteSymbolRandomOP.h"

#include "dataset/SpriteFactory.h"
#include "view/LibraryPanel.h"
#include "view/MultiSpritesImpl.h"
#include "view/GLCanvas.h"
#include "render/SpriteRenderer.h"

namespace d2d
{

PasteSymbolRandomOP::PasteSymbolRandomOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
										 LibraryPanel* libraryPanel, PasteSymbolRandomWidget* randomWidget)
	: PasteSymbolOP(editPanel, spritesImpl, libraryPanel)
	, m_randomWidget(randomWidget)
{
	changeRandomValue();
}

bool PasteSymbolRandomOP::onMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::onMouseLeftDown(x, y)) return true;

	ISymbol* symbol = m_randomValue.symbol;
	if (!symbol) 
		symbol = m_libraryPanel->GetSymbol();
	if (symbol) 
	{
		m_pos = m_stage->transPosScreenToProject(x, y);
		ISprite* sprite = SpriteFactory::Instance()->create(symbol);
		sprite->translate(m_pos);
		if (m_randomValue.scale != 1.0f) 
			sprite->setScale(m_randomValue.scale, m_randomValue.scale);
		if (m_randomValue.angle != 0.0f) 
			sprite->setTransform(m_pos, m_randomValue.angle);
		m_panelImpl->insertSprite(sprite);
		sprite->Release();
	}

	changeRandomValue();

	return false;
}

bool PasteSymbolRandomOP::onMouseRightDown(int x, int y)
{
	if (PasteSymbolOP::onMouseRightDown(x, y)) return true;

	changeRandomValue();

	return false;
}

bool PasteSymbolRandomOP::onDraw() const
{
	if (ZoomViewOP::onDraw()) return true;

	ISymbol* symbol = m_randomValue.symbol;
	if (!symbol) {
		symbol = m_libraryPanel->GetSymbol();
	}
	if (symbol && m_pos.isValid()) {
		SpriteRenderer::Instance()->Draw(symbol, Matrix(), m_pos, m_randomValue.angle, m_randomValue.scale);
	}

	return false;
}

void PasteSymbolRandomOP::changeRandomValue()
{
	m_randomWidget->getRandomValue(m_randomValue);
}

} // d2d