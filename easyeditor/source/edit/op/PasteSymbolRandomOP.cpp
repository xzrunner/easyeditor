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

	Symbol* symbol = m_random_val.symbol;
	if (!symbol) 
		symbol = m_library->GetSymbol();
	if (symbol) 
	{
		m_pos = m_stage->TransPosScrToProj(x, y);
		Sprite* sprite = SpriteFactory::Instance()->Create(symbol);
		sprite->Translate(m_pos);
		if (m_random_val.scale != 1.0f) 
			sprite->SetScale(Vector(m_random_val.scale, m_random_val.scale));
		if (m_random_val.angle != 0.0f) 
			sprite->SetTransform(m_pos, m_random_val.angle);
		InsertSpriteSJ::Instance()->Insert(sprite);
		sprite->Release();
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

	Symbol* symbol = m_random_val.symbol;
	if (!symbol) {
		symbol = m_library->GetSymbol();
	}
	if (symbol && m_pos.IsValid()) {
		SpriteRenderer::Draw(symbol, Matrix(), m_pos, m_random_val.angle, m_random_val.scale);
	}

	return false;
}

void PasteSymbolRandomOP::ChangeRandomValue()
{
	m_random_widget->GetRandomValue(m_random_val);
}

}