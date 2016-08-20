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

	Symbol* sym = m_random_value.sym;
	if (!sym) 
		sym = m_library->GetSymbol();
	if (sym) 
	{
		m_pos = m_stage->TransPosScrToProj(x, y);

		Sprite* spr = SpriteFactory::Instance()->Create(sym);
		spr->Translate(m_pos);

		if (m_random_value.scale != 1.0f) {
			spr->SetScale(sm::vec2(m_random_value.scale, m_random_value.scale));
		}
		if (m_random_value.angle != 0.0f) {
			spr->SetPosition(m_pos);
			spr->SetAngle(m_random_value.angle);
		}
		IBody* body = BodyManager::Instance()->LoadBody(spr);
		if (body) {
			body->GetBody()->SetType(m_bStatic ? b2_staticBody : b2_dynamicBody);
		}

		InsertSpriteSJ::Instance()->Insert(spr);

		spr->RemoveReference();
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

	Symbol* sym = m_random_value.sym;
	if (!sym)  {
		sym = m_library->GetSymbol();
	}
	if (sym && m_pos.IsValid()) {
		ee::SpriteRenderer::Instance()->Draw(sym, s2::RenderParams(), m_pos, m_random_value.angle, 
			sm::vec2(m_random_value.scale, m_random_value.scale));
	}

	return false;
}

void PasteSymbolPhysicsRandomOP::changeRandomValue()
{
	m_random_widget->GetRandomValue(m_random_value);
}

}
