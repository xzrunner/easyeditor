#include "SpriteDropTarget.h"

#include "common/Vector.h"
#include "dataset/ISymbol.h"
#include "dataset/ISprite.h"
#include "dataset/SpriteFactory.h"
#include "view/LibraryPanel.h"
#include "view/EditPanel.h"
#include "view/MultiSpritesImpl.h"

namespace d2d
{

SpriteDropTarget::SpriteDropTarget(MultiSpritesImpl* panelImpl, 
	EditPanel* editPanel, LibraryPanel* libraryPanel)
	: m_panelImpl(panelImpl)
	, m_stage(editPanel)
	, m_libraryPanel(libraryPanel)
{
}

bool SpriteDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& data)
{
	wxString sType = data.substr(0, data.find(","));
	wxString sIndex = data.substr(data.find(",") + 1);

	long index;
	sIndex.ToLong(&index);

	ISymbol* symbol = m_libraryPanel->getSymbol(index);
	if (symbol)
	{
		Vector pos = m_stage->transPosScreenToProject(x, y);
		ISprite* sprite = SpriteFactory::Instance()->create(symbol);
		sprite->translate(pos);
		m_panelImpl->insertSprite(sprite);
		sprite->Release();
	}

	return true;
}

}