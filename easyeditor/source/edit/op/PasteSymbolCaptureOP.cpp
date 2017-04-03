#include "PasteSymbolCaptureOP.h"
#include "SpriteFactory.h"
#include "Sprite.h"
#include "LibraryPanel.h"
#include "StageCanvas.h"
#include "EditPanelImpl.h"
#include "sprite_msg.h"
#include "panel_msg.h"

#include <SM_Calc.h>

namespace ee
{

PasteSymbolCaptureOP::PasteSymbolCaptureOP(wxWindow* wnd, EditPanelImpl* stage, LibraryPanel* library, 
										   PasteSymbolOffsetCMPT<PasteSymbolCaptureOP>* cmpt)
	: PasteSymbolOP(wnd, stage, library)
	, m_cmpt(cmpt)
	, m_bCaptured(false)
{
	m_last_pos.MakeInvalid();
}

bool PasteSymbolCaptureOP::OnMouseLeftDown(int x, int y)
{
	Symbol* sym = m_library->GetSymbol();
	if (sym) 
	{
		if (!m_bCaptured)
			m_pos = m_stage->TransPosScrToProj(x, y);
		m_last_pos = m_pos;

		Sprite* spr = SpriteFactory::Instance()->CreateRoot(sym);
		spr->Translate(m_pos);
		InsertSpriteSJ::Instance()->Insert(spr);
		spr->RemoveReference();
	}

	return false;
}

bool PasteSymbolCaptureOP::OnMouseMove(int x, int y)
{
	if (ZoomViewOP::OnMouseMove(x, y)) return true;

	m_bCaptured = false;
	m_pos = m_stage->TransPosScrToProj(x, y);
	if (m_last_pos.IsValid())
	{
		sm::vec2 offset = m_cmpt->GetOffset();
		sm::vec2 newPos = m_last_pos + offset;
		if (sm::dis_pos_to_pos(m_pos, newPos) < 
			sm::dis_pos_to_pos(offset, sm::vec2(0, 0)))
		{
			m_bCaptured = true;
			m_pos = newPos;
		}
	}
	SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool PasteSymbolCaptureOP::Clear()
{
	if (PasteSymbolOP::Clear()) return true;

	m_last_pos.MakeInvalid();

	return false;
}

}