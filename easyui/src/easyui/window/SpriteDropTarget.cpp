#include "SpriteDropTarget.h"

#include <ee/StringHelper.h>
#include <ee/Symbol.h>
#include <ee/LibraryPanel.h>
#include <ee/EditPanelImpl.h>
#include <ee/Sprite.h>
#include <ee/SpriteFactory.h>
#include <ee/sprite_msg.h>
#include <ee/panel_msg.h>
#include <ee/InsertSpriteAOP.h>
#include <ee/ImageSymbol.h>

#include <easytext.h>

namespace eui
{
namespace window
{

SpriteDropTarget::SpriteDropTarget(ee::EditPanelImpl* stage, 
								   ee::LibraryPanel* library)
	: m_stage(stage)
	, m_library(library)
{
}

bool SpriteDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& data)
{
	std::string sType = data.substr(0, data.find(","));
	std::string sIndex = data.substr(data.find(",") + 1);

	int index;
	ee::StringHelper::FromString(sIndex, index);
	ee::Symbol* sym = m_library->GetSymbol(index);
	if (!sym) {
		return true;
	}

	if (dynamic_cast<ee::ImageSymbol*>(sym) != NULL ||
		dynamic_cast<etext::Symbol*>(sym) != NULL) {
		wxMessageBox("Shouldn't use image or text symbol in ui window!");
		return true;
	}

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	ee::Sprite* spr = ee::SpriteFactory::Instance()->Create(sym);
	spr->Translate(pos);
	ee::InsertSpriteSJ::Instance()->Insert(spr);
	ee::EditAddRecordSJ::Instance()->Add(new ee::InsertSpriteAOP(spr));
	spr->RemoveReference();

	return true;
}

}
}