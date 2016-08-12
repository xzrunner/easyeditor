#include "SpriteDropTarget.h"
#include "Symbol.h"
#include "Sprite.h"
#include "SpriteFactory.h"
#include "LibraryPanel.h"
#include "EditPanelImpl.h"
#include "sprite_msg.h"
#include "StringHelper.h"
#include "panel_msg.h"
#include "InsertSpriteAOP.h"

namespace ee
{

SpriteDropTarget::SpriteDropTarget(EditPanelImpl* stage, 
								   LibraryPanel* library)
	: m_stage(stage)
	, m_library(library)
{
}

bool SpriteDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& data)
{
	std::string sType = data.substr(0, data.find(","));
	std::string sIndex = data.substr(data.find(",") + 1);

	int index;
	StringHelper::FromString(sIndex, index);
	Symbol* symbol = m_library->GetSymbol(index);
	if (symbol)
	{
		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
		Sprite* sprite = SpriteFactory::Instance()->Create(symbol);
		sprite->Translate(pos);
		InsertSpriteSJ::Instance()->Insert(sprite);
		EditAddRecordSJ::Instance()->Add(new InsertSpriteAOP(sprite));
		sprite->RemoveReference();
	}

	return true;
}

}