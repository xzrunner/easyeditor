#include "PasteSymbolRandomOP.h"

#include "dataset/SpriteFactory.h"
#include "dataset/Sprite.h"
#include "view/LibraryPanel.h"
#include "view/IStageCanvas.h"
#include "view/EditPanelImpl.h"
#include "render/SpriteRenderer.h"
#include "message/InsertSpriteSJ.h"

namespace d2d
{

PasteSymbolRandomOP::PasteSymbolRandomOP(wxWindow* wnd, EditPanelImpl* stage, LibraryPanel* libraryPanel, 
										 PasteSymbolRandomWidget* randomWidget)
	: PasteSymbolOP(wnd, stage, libraryPanel)
	, m_randomWidget(randomWidget)
{
	changeRandomValue();
}

bool PasteSymbolRandomOP::OnMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	Symbol* symbol = m_randomValue.symbol;
	if (!symbol) 
		symbol = m_libraryPanel->GetSymbol();
	if (symbol) 
	{
		m_pos = m_stage->TransPosScrToProj(x, y);
		Sprite* sprite = SpriteFactory::Instance()->Create(symbol);
		sprite->Translate(m_pos);
		if (m_randomValue.scale != 1.0f) 
			sprite->SetScale(Vector(m_randomValue.scale, m_randomValue.scale));
		if (m_randomValue.angle != 0.0f) 
			sprite->SetTransform(m_pos, m_randomValue.angle);
		InsertSpriteSJ::Instance()->Insert(sprite);
		sprite->Release();
	}

	changeRandomValue();

	return false;
}

bool PasteSymbolRandomOP::OnMouseRightDown(int x, int y)
{
	if (PasteSymbolOP::OnMouseRightDown(x, y)) return true;

	changeRandomValue();

	return false;
}

bool PasteSymbolRandomOP::OnDraw() const
{
	if (ZoomViewOP::OnDraw()) return true;

	Symbol* symbol = m_randomValue.symbol;
	if (!symbol) {
		symbol = m_libraryPanel->GetSymbol();
	}
	if (symbol && m_pos.IsValid()) {
		SpriteRenderer::Instance()->Draw(symbol, Matrix(), m_pos, m_randomValue.angle, m_randomValue.scale);
	}

	return false;
}

void PasteSymbolRandomOP::changeRandomValue()
{
	m_randomWidget->getRandomValue(m_randomValue);
}

}