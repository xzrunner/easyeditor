#include "PasteSymbolPhysicsRandomOP.h"
#include "BodyManager.h"
#include "IBody.h"
#include "LibraryPanel.h"
#include "EditPanelImpl.h"
#include "SpriteFactory.h"
#include "Sprite.h"
#include "sprite_msg.h"
#include "SpriteRenderer.h"

namespace ee
{

PasteSymbolPhysicsRandomOP::PasteSymbolPhysicsRandomOP(wxWindow* stage_wnd, EditPanelImpl* stage, 
													   LibraryPanel* library, PhysicsPanelImpl* physicsImpl, 
													   PasteSymbolRandomWidget* randomWidget)
	: PasteSymbolPhysicsOP(stage_wnd, stage, library, physicsImpl)
	, m_random_widget(randomWidget)
{
	changeRandomValue();
}

bool PasteSymbolPhysicsRandomOP::OnMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	Symbol* symbol = m_random_value.symbol;
	if (!symbol) 
		symbol = m_library->GetSymbol();
	if (symbol) 
	{
		m_pos = m_stage->TransPosScrToProj(x, y);

		Sprite* sprite = SpriteFactory::Instance()->Create(symbol);
		sprite->Translate(m_pos);

		if (m_random_value.scale != 1.0f) {
			sprite->SetScale(sm::vec2(m_random_value.scale, m_random_value.scale));
		}
		if (m_random_value.angle != 0.0f) {
			sprite->SetTransform(m_pos, m_random_value.angle);
		}
		IBody* body = BodyManager::Instance()->LoadBody(sprite);
		if (body) {
			body->GetBody()->SetType(m_bStatic ? b2_staticBody : b2_dynamicBody);
		}

		InsertSpriteSJ::Instance()->Insert(sprite);

		sprite->RemoveReference();
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
	if (ZoomViewOP::OnDraw()) return true;

	Symbol* symbol = m_random_value.symbol;
	if (!symbol)  {
		symbol = m_library->GetSymbol();
	}
	if (symbol && m_pos.IsValid()) {
		SpriteRenderer::Draw(symbol, s2::RenderParams(), m_pos, m_random_value.angle, m_random_value.scale);
	}

	return false;
}

void PasteSymbolPhysicsRandomOP::changeRandomValue()
{
	m_random_widget->GetRandomValue(m_random_value);
}

}
