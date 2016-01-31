#include "SpriteDropTarget.h"
#include "Vector.h"
#include "Symbol.h"
#include "Sprite.h"
#include "SpriteFactory.h"
#include "LibraryPanel.h"
#include "EditPanelImpl.h"
#include "sprite_msg.h"
#include "StringHelper.h"

namespace ee
{

SpriteDropTarget::SpriteDropTarget(EditPanelImpl* stage, 
								   LibraryPanel* libraryPanel)
	: m_stage(stage)
	, m_libraryPanel(libraryPanel)
{
}

bool SpriteDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& data)
{
	std::string sType = data.substr(0, data.find(","));
	std::string sIndex = data.substr(data.find(",") + 1);

	int index;
	StringHelper::FromString(sIndex, index);
	Symbol* symbol = m_libraryPanel->GetSymbol(index);
	if (symbol)
	{
		Vector pos = m_stage->TransPosScrToProj(x, y);
		Sprite* sprite = SpriteFactory::Instance()->Create(symbol);
		sprite->Translate(pos);
		InsertSpriteSJ::Instance()->Insert(sprite);
		sprite->Release();
	}

	return true;
}

}