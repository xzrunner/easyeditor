#include "StagePanel.h"
#include "LibraryPanel.h"
#include "StageCanvas.h"
#include "Context.h"

namespace coceditor
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(this, Context::Instance()->library)
{
	m_editOP = new ee::ArrangeSpriteOP<ee::SelectSpritesOP>(this, this, Context::Instance()->property);
	m_canvas = new StageCanvas(this);

	SetDropTarget(new DragSymbolTarget(this));
}

StagePanel::~StagePanel()
{
	clear();
}

void StagePanel::clear()
{
	ee::EditPanel::clear();
	ee::SpritesPanelImpl::clearSprites();
}

void StagePanel::insertSprite(ee::ISprite* sprite)
{
	ee::SpritesPanelImpl::insertSprite(sprite);
	if (!sprite->getUserData())
		sprite->setUserData(new int(Context::Instance()->id++));
}

//////////////////////////////////////////////////////////////////////////
// class StagePanel::DragSymbolTarget
//////////////////////////////////////////////////////////////////////////

StagePanel::DragSymbolTarget::DragSymbolTarget(StagePanel* stage)
	: m_stage(stage)
{
}

bool StagePanel::DragSymbolTarget::OnDropText(wxCoord x, wxCoord y, const wxString& data)
{
	wxString sType = data.substr(0, data.find(","));
	wxString sIndex = data.substr(data.find(",") + 1);

	long index;
	sIndex.ToLong(&index);

	LibraryPanel* library = Context::Instance()->library;

	ee::ISymbol* symbol = NULL;
	if (sType == "symbol")
		symbol = static_cast<ee::ISymbol*>(library->getImagePage()->getSymbol(index));
	else if (sType == "complex")
		symbol = static_cast<ee::ISymbol*>(library->getComplexPage()->getSymbol(index));
	else if (sType == "anim")
		symbol = static_cast<ee::ISymbol*>(library->getAnimPage()->getSymbol(index));
	else if (sType == "scale9")
		symbol = static_cast<ee::ISymbol*>(library->get9PatchPage()->getSymbol(index));

	if (symbol)
	{
		ee::Vector pos = m_stage->transPosScreenToProject(x, y);
		ee::ISprite* sprite = ee::SpriteFactory::Instance()->create(symbol);
		sprite->translate(pos);
		m_stage->insertSprite(sprite);
	}

	return true;
}
} // coceditor