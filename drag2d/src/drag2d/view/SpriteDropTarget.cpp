#include "SpriteDropTarget.h"

#include "common/Vector.h"
#include "dataset/ISymbol.h"
#include "dataset/ISprite.h"
#include "dataset/SpriteFactory.h"
#include "view/LibraryPanel.h"
#include "view/EditPanelImpl.h"
#include "message/InsertSpriteSJ.h"

namespace d2d
{

SpriteDropTarget::SpriteDropTarget(EditPanelImpl* stage, 
								   LibraryPanel* libraryPanel)
	: m_stage(stage)
	, m_libraryPanel(libraryPanel)
{
}

bool SpriteDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& data)
{
	wxString sType = data.substr(0, data.find(","));
	wxString sIndex = data.substr(data.find(",") + 1);

	long index;
	sIndex.ToLong(&index);

	ISymbol* symbol = m_libraryPanel->GetSymbol(index);
	if (symbol)
	{
		Vector pos = m_stage->TransPosScrToProj(x, y);
		ISprite* sprite = SpriteFactory::Instance()->create(symbol);
		sprite->Translate(pos);
		InsertSpriteSJ::Instance()->Insert(sprite);
		sprite->Release();
	}

	return true;
}

}