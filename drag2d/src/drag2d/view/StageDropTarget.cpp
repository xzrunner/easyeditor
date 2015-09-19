#include "StageDropTarget.h"
#include "MultiSpritesImpl.h"
#include "LibraryPanel.h"

#include "common/StringTools.h"
#include "dataset/SpriteFactory.h"
#include "dataset/SymbolMgr.h"
#include "dataset/ISprite.h"
#include "view/EditPanelImpl.h"

namespace d2d
{

StageDropTarget::StageDropTarget(wxWindow* stage_wnd, EditPanelImpl* stage, 
								 MultiSpritesImpl* sprites_impl, 
								 LibraryPanel* library)
	: CombinedDropTarget(stage_wnd)
	, m_stage(stage)
	, m_sprites_impl(sprites_impl)
	, m_library(library)
{
}

void StageDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& text)
{
	std::vector<std::string> keys;
	StringTools::Split(text.ToStdString(), ",", keys);

	if (keys.size() <= 1) {
		return;
	}

	for (int i = 1, n = keys.size(); i < n; ++i)
	{
		int idx = StringTools::FromString<int>(keys[i]);
		ISymbol* symbol = m_library->GetSymbol(idx);
		if (!symbol) {
			continue;
		}

		Vector pos = m_stage->TransPosScrToProj(x, y);
		bool handled = OnDropSymbol(symbol, pos);
		if (handled) {
			continue;
		}

		if (m_sprites_impl) {
			ISprite* sprite = SpriteFactory::Instance()->create(symbol);
			if (sprite->GetSymbol().GetSize().isValid()) {
				sprite->Translate(pos);
				m_sprites_impl->InsertSprite(sprite);
			}
			sprite->Release();
		}
	}
}

void StageDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
	for (int i = 0, n = filenames.size(); i < n; ++i)
	{
		std::string filename = filenames[i].ToStdString();
		ISymbol* symbol = SymbolMgr::Instance()->FetchSymbol(filename);
		symbol->RefreshThumbnail(filename);
		bool success = m_library->AddSymbol(symbol);
		if (success) {
			Vector pos = m_stage->TransPosScrToProj(x, y);
			bool handled = OnDropSymbol(symbol, pos);
			if (handled) {
				continue;
			}

			if (m_sprites_impl) {
				ISprite* sprite = SpriteFactory::Instance()->create(symbol);
				sprite->Translate(pos);
				m_sprites_impl->InsertSprite(sprite);
				sprite->Release();
			}
		}
		symbol->Release();
	}
}

}