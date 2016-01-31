#include "PasteSymbolCaptureOP.h"
#include "Math2D.h"
#include "SpriteFactory.h"
#include "Sprite.h"
#include "LibraryPanel.h"
#include "StageCanvas.h"
#include "EditPanelImpl.h"
#include "sprite_msg.h"
#include "panel_msg.h"

namespace ee
{

PasteSymbolCaptureOP::PasteSymbolCaptureOP(wxWindow* wnd, EditPanelImpl* stage, LibraryPanel* libraryPanel, 
										   PasteSymbolOffsetCMPT<PasteSymbolCaptureOP>* cmpt)
	: PasteSymbolOP(wnd, stage, libraryPanel)
	, m_cmpt(cmpt)
	, m_bCaptured(false)
{
	m_lastPos.SetInvalid();
}

bool PasteSymbolCaptureOP::OnMouseLeftDown(int x, int y)
{
	Symbol* symbol = m_library->GetSymbol();
	if (symbol) 
	{
		if (!m_bCaptured)
			m_pos = m_stage->TransPosScrToProj(x, y);
		m_lastPos = m_pos;

		Sprite* sprite = SpriteFactory::Instance()->Create(symbol);
		sprite->Translate(m_pos);
		InsertSpriteSJ::Instance()->Insert(sprite);
		sprite->Release();
	}

	return false;
}

bool PasteSymbolCaptureOP::OnMouseMove(int x, int y)
{
	if (ZoomViewOP::OnMouseMove(x, y)) return true;

	m_bCaptured = false;
	m_pos = m_stage->TransPosScrToProj(x, y);
	if (m_lastPos.IsValid())
	{
		Vector offset = m_cmpt->GetOffset();
		Vector newPos = m_lastPos + offset;
		if (Math2D::GetDistance(m_pos, newPos) < 
			Math2D::GetDistance(offset, Vector(0, 0)))
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

	m_lastPos.SetInvalid();

	return false;
}

}