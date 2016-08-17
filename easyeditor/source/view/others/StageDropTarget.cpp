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
		Symbol* sym = m_library->GetSymbol(idx);
		if (!sym) {
			continue;
		}

		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
		bool handled = OnDropSymbol(sym, pos);
		if (handled) {
			continue;
		}

		Sprite* spr = SpriteFactory::Instance()->Create(sym);
		if (spr->GetSymbol()->GetBounding().IsValid()) {
			spr->Translate(pos);
			InsertSpriteSJ::Instance()->Insert(spr);
		}
		spr->RemoveReference();
	}
}

void StageDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
	for (int i = 0, n = filenames.size(); i < n; ++i)
	{
		std::string filename = filenames[i].ToStdString();
		Symbol* sym = SymbolMgr::Instance()->FetchSymbol(filename);
		sym->RefreshThumbnail(filename);
		bool success = m_library->AddSymbol(sym);
		if (success) {
			sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
			bool handled = OnDropSymbol(sym, pos);
			if (handled) {
				continue;
			}

			Sprite* spr = SpriteFactory::Instance()->Create(sym);
			spr->Translate(pos);
			InsertSpriteSJ::Instance()->Insert(spr);
			spr->RemoveReference();
		}
		sym->RemoveReference();
	}
}

}