#include "PasteSymbolRandomOP.h"
#include "SpriteFactory.h"
#include "Sprite.h"
#include "LibraryPanel.h"
#include "StageCanvas.h"
#include "EditPanelImpl.h"
#include "SpriteRenderer.h"
#include "sprite_msg.h"

namespace ee
{

PasteSymbolRandomOP::PasteSymbolRandomOP(wxWindow* wnd, EditPanelImpl* stage, LibraryPanel* library, 
										 PasteSymbolRandomWidget* randomWidget)
	: PasteSymbolOP(wnd, stage, library)
	, m_random_widget(randomWidget)
{
	ChangeRandomValue();
}

bool PasteSymbolRandomOP::OnMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	auto sym = m_random_val.sym;
	if (!sym) 
		sym = m_library->GetSymbol();
	if (sym) 
	{
		m_pos = m_stage->TransPosScrToProj(x, y);
		auto spr = SpriteFactory::Instance()->CreateRoot(sym);
		spr->Translate(m_pos);
		if (m_random_val.scale != 1.0f) {
			spr->SetScale(sm::vec2(m_random_val.scale, m_random_val.scale));
		}
		if (m_random_val.angle != 0.0f) {
			spr->SetPosition(m_pos);
			spr->SetAngle(m_random_val.angle);
		}
		InsertSpriteSJ::Instance()->Insert(spr);
	}

	ChangeRandomValue();

	return false;
}

bool PasteSymbolRandomOP::OnMouseRightDown(int x, int y)
{
	if (PasteSymbolOP::OnMouseRightDown(x, y)) return true;

	ChangeRandomValue();

	return false;
}

bool PasteSymbolRandomOP::OnDraw() const
{
	if (ZoomViewOP::OnDraw()) return true;

	auto sym = m_random_val.sym;
	if (!sym) {
		sym = m_library->GetSymbol();
	}
	if (sym && m_pos.IsValid()) {
		ee::SpriteRenderer::Instance()->Draw(sym.get(), s2::RenderParams(), m_pos, m_random_val.angle, 
			sm::vec2(m_random_val.scale, m_random_val.scale));
	}

	return false;
}

void PasteSymbolRandomOP::ChangeRandomValue()
{
	m_random_widget->GetRandomValue(m_random_val);
}

}