#include "PasteSymbolPhysicsRandomOP.h"
#include "BodyManager.h"
#include "IBody.h"

namespace ephysics
{

PasteSymbolPhysicsRandomOP::PasteSymbolPhysicsRandomOP(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, 
													   d2d::LibraryPanel* libraryPanel, PhysicsPanelImpl* physicsImpl, 
													   d2d::PasteSymbolRandomWidget* randomWidget)
	: PasteSymbolPhysicsOP(stage_wnd, stage, libraryPanel, physicsImpl)
	, m_randomWidget(randomWidget)
{
	changeRandomValue();
}

bool PasteSymbolPhysicsRandomOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	d2d::Symbol* symbol = m_randomValue.symbol;
	if (!symbol) 
		symbol = m_libraryPanel->GetSymbol();
	if (symbol) 
	{
		m_pos = m_stage->TransPosScrToProj(x, y);

		d2d::Sprite* sprite = d2d::SpriteFactory::Instance()->Create(symbol);
		sprite->Translate(m_pos);

		if (m_randomValue.scale != 1.0f) {
			sprite->SetScale(d2d::Vector(m_randomValue.scale, m_randomValue.scale));
		}
		if (m_randomValue.angle != 0.0f) {
			sprite->SetTransform(m_pos, m_randomValue.angle);
		}
		IBody* body = BodyManager::Instance()->LoadBody(sprite);
		if (body) {
			body->getBody()->SetType(m_bStatic ? b2_staticBody : b2_dynamicBody);
		}

		d2d::InsertSpriteSJ::Instance()->Insert(sprite);

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

	d2d::Symbol* symbol = m_randomValue.symbol;
	if (!symbol)  {
		symbol = m_libraryPanel->GetSymbol();
	}
	if (symbol && m_pos.IsValid()) {
		d2d::SpriteRenderer::Instance()->Draw(symbol, d2d::Matrix(), m_pos, m_randomValue.angle, m_randomValue.scale);
	}

	return false;
}

void PasteSymbolPhysicsRandomOP::changeRandomValue()
{
	m_randomWidget->getRandomValue(m_randomValue);
}

}
