#include "PasteSymbolPhysicsRandomOP.h"
#include "BodyManager.h"
#include "IBody.h"

namespace ephysics
{

PasteSymbolPhysicsRandomOP::PasteSymbolPhysicsRandomOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
													   d2d::LibraryPanel* libraryPanel, PhysicsPanelImpl* physicsImpl, 
													   d2d::PasteSymbolRandomWidget* randomWidget)
	: PasteSymbolPhysicsOP(editPanel, spritesImpl, libraryPanel, physicsImpl)
	, m_randomWidget(randomWidget)
{
	changeRandomValue();
}

bool PasteSymbolPhysicsRandomOP::onMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftDown(x, y)) return true;

	d2d::ISymbol* symbol = m_randomValue.symbol;
	if (!symbol) 
		symbol = m_libraryPanel->getSymbol();
	if (symbol) 
	{
		m_pos = m_editPanel->transPosScreenToProject(x, y);

		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		sprite->translate(m_pos);

		if (m_randomValue.scale != 1.0f) {
			sprite->setScale(m_randomValue.scale, m_randomValue.scale);
		}
		if (m_randomValue.angle != 0.0f) {
			sprite->setTransform(m_pos, m_randomValue.angle);
		}
		IBody* body = BodyManager::Instance()->LoadBody(sprite);
		if (body) {
			body->getBody()->SetType(m_bStatic ? b2_staticBody : b2_dynamicBody);
		}
		m_panelImpl->insertSprite(sprite);
		sprite->Release();
	}

	changeRandomValue();

	return false;
}

bool PasteSymbolPhysicsRandomOP::onMouseRightDown(int x, int y)
{
	if (PasteSymbolPhysicsOP::onMouseRightDown(x, y)) return true;

	changeRandomValue();

	return false;
}

bool PasteSymbolPhysicsRandomOP::onDraw() const
{
	if (d2d::ZoomViewOP::onDraw()) return true;

	d2d::ISymbol* symbol = m_randomValue.symbol;
	if (!symbol)  {
		symbol = m_libraryPanel->getSymbol();
	}
	if (symbol && m_pos.isValid()) {
		d2d::SpriteDraw::drawSprite(symbol, d2d::Matrix(), m_pos, m_randomValue.angle, m_randomValue.scale);
	}

	return false;
}

void PasteSymbolPhysicsRandomOP::changeRandomValue()
{
	m_randomWidget->getRandomValue(m_randomValue);
}

}
