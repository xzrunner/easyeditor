#include "PasteSymbolOP.h"
#include "SpriteFactory.h"
#include "Sprite.h"
#include "LibraryPanel.h"
#include "StageCanvas.h"
#include "EditPanelImpl.h"
#include "SpriteRenderer.h"
#include "sprite_msg.h"
#include "panel_msg.h"

#include <sprite2/DrawNode.h>

namespace ee
{

PasteSymbolOP::PasteSymbolOP(wxWindow* wnd, EditPanelImpl* stage,
							 LibraryPanel* library, float* pScale/* = NULL*/)
	: ZoomViewOP(wnd, stage, true)
	, m_library(library)
	, m_scale(pScale)
{
	m_pos.MakeInvalid();
	m_cursor = wxCursor(wxCURSOR_PAINT_BRUSH);
}

bool PasteSymbolOP::OnMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	Symbol* sym = m_library->GetSymbol();
	if (sym)
	{
		m_pos = m_stage->TransPosScrToProj(x, y);
		Sprite* spr = SpriteFactory::Instance()->Create(sym);
		spr->Translate(m_pos);
		if (m_scale) {
			spr->SetScale(sm::vec2(*m_scale, *m_scale));
		}
		InsertSpriteSJ::Instance()->Insert(spr);
		spr->RemoveReference();
	}

	return false;
}

bool PasteSymbolOP::OnMouseMove(int x, int y)
{
	if (ZoomViewOP::OnMouseMove(x, y)) return true;

	m_pos = m_stage->TransPosScrToProj(x, y);
	SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool PasteSymbolOP::OnDraw() const
{
	if (ZoomViewOP::OnDraw()) return true;

	Symbol* sym = m_library->GetSymbol();
	if (sym && m_pos.IsValid())
	{
		if (m_scale) {
			s2::DrawNode::Draw(sym, s2::RenderParams(), m_pos, 0.0f, sm::vec2(*m_scale, *m_scale));
		} else {
			s2::DrawNode::Draw(sym, s2::RenderParams(), m_pos);
		}
	}

	return false;
}

bool PasteSymbolOP::Clear()
{
	if (ZoomViewOP::Clear()) return true;

	m_pos.MakeInvalid();

	return false;
}

}