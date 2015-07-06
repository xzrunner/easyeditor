#include "StageDropTarget.h"
#include "EditPanel.h"
#include "MultiSpritesImpl.h"
#include "LibraryPanel.h"

#include "dataset/SpriteFactory.h"
#include "dataset/SymbolMgr.h"

namespace d2d
{

StageDropTarget::StageDropTarget(EditPanel* edit_panel, 
								 MultiSpritesImpl* sprites_impl, 
								 LibraryPanel* library)
	: CombinedDropTarget(edit_panel)
	, m_edit_panel(edit_panel)
	, m_sprites_impl(sprites_impl)
	, m_library(library)
{
}

void StageDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& text)
{
	wxString sType = text.substr(0, text.find(","));
	wxString sIndex = text.substr(text.find(",") + 1);

	long index;
	sIndex.ToLong(&index);

	ISymbol* symbol = m_library->GetSymbol(index);
	if (symbol)
	{
		Vector pos = m_edit_panel->transPosScreenToProject(x, y);
		bool handled = OnDropSymbol(symbol, pos);
		if (!handled) {
			ISprite* sprite = SpriteFactory::Instance()->create(symbol);
			if (sprite->getSymbol().GetSize().isValid()) {
				sprite->translate(pos);
				m_sprites_impl->insertSprite(sprite);
			}
			sprite->Release();
		}
	}
}

void StageDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
	for (int i = 0, n = filenames.size(); i < n; ++i)
	{
		wxString filename = filenames[i];
		ISymbol* symbol = SymbolMgr::Instance()->fetchSymbol(filename);
		symbol->RefreshThumbnail(filename);
		bool success = m_library->AddSymbol(symbol);
		if (success) {
			Vector pos = m_edit_panel->transPosScreenToProject(x, y);
			ISprite* sprite = SpriteFactory::Instance()->create(symbol);
			sprite->translate(pos);
			m_sprites_impl->insertSprite(sprite);
			sprite->Release();
		}
		symbol->Release();
	}
}

}