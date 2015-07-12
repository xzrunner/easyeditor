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

bool PasteSymbolPhysicsRandomOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	d2d::ISymbol* symbol = m_randomValue.symbol;
	if (!symbol) 
		symbol = m_libraryPanel->GetSymbol();
	if (symbol) 
	{
		m_pos = m_stage->TransPosScrToProj(x, y);

		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		sprite->Translate(m_pos);

		if (m_randomValue.scale != 1.0f) {
			sprite->SetScale(m_randomValue.scale, m_randomValue.scale);
		}
		if (m_randomValue.angle != 0.0f) {
			sprite->SetTransform(m_pos, m_randomValue.angle);
		}
		IBody* body = BodyManager::Instance()->LoadBody(sprite);
		if (body) {
			body->getBody()->SetType(m_bStatic ? b2_staticBody : b2_dynamicBody);
		}
		m_panelImpl->InsertSprite(sprite);
		sprite->Release();
	}

	changeRandomValue();

	return false;
}

bool PasteSymbolPhysicsRandomOP::OnMouseRightDown(int x, int y)
{
	if (PasteSymbolPhysicsOP::OnMouseRightDown(x, y)) return true;

	changeRandomValue();

	return false;
}

bool PasteSymbolPhysicsRandomOP::OnDraw() const
{
	if (d2d::ZoomViewOP::OnDraw()) return true;

	d2d::ISymbol* symbol = m_randomValue.symbol;
	if (!symbol)  {
		symbol = m_libraryPanel->GetSymbol();
	}
	if (symbol && m_pos.isValid()) {
		d2d::SpriteRenderer::Instance()->Draw(symbol, d2d::Matrix(), m_pos, m_randomValue.angle, m_randomValue.scale);
	}

	return false;
}

void PasteSymbolPhysicsRandomOP::changeRandomValue()
{
	m_randomWidget->getRandomValue(m_randomValue);
}

}
