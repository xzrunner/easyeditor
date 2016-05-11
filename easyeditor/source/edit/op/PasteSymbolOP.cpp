#include "PasteSymbolOP.h"
#include "SpriteFactory.h"
#include "Sprite.h"
#include "LibraryPanel.h"
#include "StageCanvas.h"
#include "EditPanelImpl.h"
#include "SpriteRenderer.h"
#include "sprite_msg.h"
#include "panel_msg.h"

namespace ee
{

PasteSymbolOP::PasteSymbolOP(wxWindow* wnd, EditPanelImpl* stage,
							 LibraryPanel* library, float* pScale/* = NULL*/)
	: ZoomViewOP(wnd, stage, true)
	, m_library(library)
	, m_scale(pScale)
	, m_pos_valid(false)
{
	m_cursor = wxCursor(wxCURSOR_PAINT_BRUSH);
}

bool PasteSymbolOP::OnMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	Symbol* symbol = m_library->GetSymbol();
	if (symbol)
	{
		m_pos = m_stage->TransPosScrToProj(x, y);
		m_pos_valid = true;
		Sprite* sprite = SpriteFactory::Instance()->Create(symbol);
		sprite->Translate(m_pos);
		if (m_scale) {
			sprite->SetScale(sm::vec2(*m_scale, *m_scale));
		}
		InsertSpriteSJ::Instance()->Insert(sprite);
		sprite->Release();
	}

	return false;
}

bool PasteSymbolOP::OnMouseMove(int x, int y)
{
	if (ZoomViewOP::OnMouseMove(x, y)) return true;

	m_pos = m_stage->TransPosScrToProj(x, y);
	m_pos_valid = true;
	SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool PasteSymbolOP::OnDraw() const
{
	if (ZoomViewOP::OnDraw()) return true;

	Symbol* symbol = m_library->GetSymbol();
	if (symbol && m_pos_valid)
	{
		if (m_scale) {
			SpriteRenderer::Draw(symbol, s2::RenderParams(), m_pos, 0.0f, *m_scale);
		} else {
			SpriteRenderer::Draw(symbol, s2::RenderParams(), m_pos);
		}
	}

	return false;
}

bool PasteSymbolOP::Clear()
{
	if (ZoomViewOP::Clear()) return true;

	m_pos_valid = false;

	return false;
}

}