#include "PasteSymbolPhysicsRandomOP.h"
#include "BodyManager.h"
#include "IBody.h"

namespace ephysics
{

PasteSymbolPhysicsRandomOP::PasteSymbolPhysicsRandomOP(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
													   ee::LibraryPanel* libraryPanel, PhysicsPanelImpl* physicsImpl, 
													   ee::PasteSymbolRandomWidget* randomWidget)
	: PasteSymbolPhysicsOP(stage_wnd, stage, libraryPanel, physicsImpl)
	, m_randomWidget(randomWidget)
{
	changeRandomValue();
}

bool PasteSymbolPhysicsRandomOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	ee::Symbol* symbol = m_randomValue.symbol;
	if (!symbol) 
		symbol = m_libraryPanel->GetSymbol();
	if (symbol) 
	{
		m_pos = m_stage->TransPosScrToProj(x, y);

		ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
		sprite->Translate(m_pos);

		if (m_randomValue.scale != 1.0f) {
			sprite->SetScale(ee::Vector(m_randomValue.scale, m_randomValue.scale));
		}
		if (m_randomValue.angle != 0.0f) {
			sprite->SetTransform(m_pos, m_randomValue.angle);
		}
		IBody* body = BodyManager::Instance()->LoadBody(sprite);
		if (body) {
			body->getBody()->SetType(m_bStatic ? b2_staticBody : b2_dynamicBody);
		}

		ee::InsertSpriteSJ::Instance()->Insert(sprite);

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
	if (ee::ZoomViewOP::OnDraw()) return true;

	ee::Symbol* symbol = m_randomValue.symbol;
	if (!symbol)  {
		symbol = m_libraryPanel->GetSymbol();
	}
	if (symbol && m_pos.IsValid()) {
		ee::SpriteRenderer::Instance()->Draw(symbol, ee::Matrix(), m_pos, m_randomValue.angle, m_randomValue.scale);
	}

	return false;
}

void PasteSymbolPhysicsRandomOP::changeRandomValue()
{
	m_randomWidget->getRandomValue(m_randomValue);
}

}
