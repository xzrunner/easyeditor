#include "PasteSymbolOP.h"

#include "dataset/SpriteFactory.h"
#include "dataset/ISprite.h"
#include "view/LibraryPanel.h"
#include "view/MultiSpritesImpl.h"
#include "view/IStageCanvas.h"
#include "view/EditPanelImpl.h"
#include "render/SpriteRenderer.h"

namespace d2d
{

PasteSymbolOP::PasteSymbolOP(wxWindow* wnd, d2d::EditPanelImpl* stage, MultiSpritesImpl* panelImpl, 
							 LibraryPanel* libraryPanel, float* pScale/* = NULL*/)
	: ZoomViewOP(wnd, stage, true)
	, m_panelImpl(panelImpl)
	, m_libraryPanel(libraryPanel)
	, m_pScale(pScale)
{
	m_cursor = wxCursor(wxCURSOR_PAINT_BRUSH);

	m_pos.setInvalid();
}

bool PasteSymbolOP::OnMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	ISymbol* symbol = m_libraryPanel->GetSymbol();
	if (symbol)
	{
		m_pos = m_stage->TransPosScrToProj(x, y);
		ISprite* sprite = SpriteFactory::Instance()->create(symbol);
		sprite->Translate(m_pos);
		if (m_pScale)
			sprite->SetScale(*m_pScale, *m_pScale);
		m_panelImpl->InsertSprite(sprite);
		sprite->Release();
	}

	return false;
}

bool PasteSymbolOP::OnMouseMove(int x, int y)
{
	if (ZoomViewOP::OnMouseMove(x, y)) return true;

	m_pos = m_stage->TransPosScrToProj(x, y);
	m_stage->SetCanvasDirty();

	return false;
}

bool PasteSymbolOP::OnDraw() const
{
	if (ZoomViewOP::OnDraw()) return true;

	ISymbol* symbol = m_libraryPanel->GetSymbol();
	if (symbol && m_pos.isValid())
	{
		SpriteRenderer* rd = SpriteRenderer::Instance();
		if (m_pScale) {
			rd->Draw(symbol, Matrix(), m_pos, 0.0f, *m_pScale);
		} else {
			rd->Draw(symbol, Matrix(), m_pos);
		}
	}

	return false;
}

bool PasteSymbolOP::Clear()
{
	if (ZoomViewOP::Clear()) return true;

	m_pos.setInvalid();

	return false;
}

} // d2d