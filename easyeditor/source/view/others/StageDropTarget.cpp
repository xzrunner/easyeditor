#include "StageDropTarget.h"
#include "LibraryPanel.h"
#include "StringHelper.h"
#include "SpriteFactory.h"
#include "SymbolMgr.h"
#include "Sprite.h"
#include "EditPanelImpl.h"
#include "sprite_msg.h"

namespace ee
{

StageDropTarget::StageDropTarget(wxWindow* stage_wnd, EditPanelImpl* stage, 
								 LibraryPanel* library)
	: CombinedDropTarget(stage_wnd)
	, m_stage(stage)
	, m_library(library)
{
}

void StageDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& text)
{
	std::vector<std::string> keys;
	StringHelper::Split(text.ToStdString(), ",", keys);

	if (keys.size() <= 1) {
		return;
	}

	for (int i = 1, n = keys.size(); i < n; ++i)
	{
		int idx = StringHelper::FromString<int>(keys[i]);
		Symbol* symbol = m_library->GetSymbol(idx);
		if (!symbol) {
			continue;
		}

		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
		bool handled = OnDropSymbol(symbol, pos);
		if (handled) {
			continue;
		}

		Sprite* sprite = SpriteFactory::Instance()->Create(symbol);
		if (sprite->GetSymbol().GetBounding().IsValid()) {
			sprite->Translate(pos);
			InsertSpriteSJ::Instance()->Insert(sprite);
		}
		sprite->RemoveReference();
	}
}

void StageDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
	for (int i = 0, n = filenames.size(); i < n; ++i)
	{
		std::string filename = filenames[i].ToStdString();
		Symbol* symbol = SymbolMgr::Instance()->FetchSymbol(filename);
		symbol->RefreshThumbnail(filename);
		bool success = m_library->AddSymbol(symbol);
		if (success) {
			sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
			bool handled = OnDropSymbol(symbol, pos);
			if (handled) {
				continue;
			}

			Sprite* sprite = SpriteFactory::Instance()->Create(symbol);
			sprite->Translate(pos);
			InsertSpriteSJ::Instance()->Insert(sprite);
			sprite->RemoveReference();
		}
		symbol->RemoveReference();
	}
}

}