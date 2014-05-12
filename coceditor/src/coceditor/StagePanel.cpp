#include "StagePanel.h"
#include "LibraryPanel.h"
#include "StageCanvas.h"
#include "Context.h"

namespace coceditor
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, Context::Instance()->library)
{
	m_editOP = new d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(this, this, Context::Instance()->property);
	m_canvas = new StageCanvas(this);

	SetDropTarget(new DragSymbolTarget(this));
}

StagePanel::~StagePanel()
{
	clear();
}

void StagePanel::clear()
{
	d2d::EditPanel::clear();
	d2d::SpritesPanelImpl::clearSprites();
}

void StagePanel::insertSprite(d2d::ISprite* sprite)
{
	d2d::SpritesPanelImpl::insertSprite(sprite);
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

	d2d::ISymbol* symbol = NULL;
	if (sType == "symbol")
		symbol = static_cast<d2d::ISymbol*>(library->getImagePage()->getSymbol(index));
	else if (sType == "complex")
		symbol = static_cast<d2d::ISymbol*>(library->getComplexPage()->getSymbol(index));
	else if (sType == "anim")
		symbol = static_cast<d2d::ISymbol*>(library->getAnimPage()->getSymbol(index));
	else if (sType == "scale9")
		symbol = static_cast<d2d::ISymbol*>(library->get9PatchPage()->getSymbol(index));

	if (symbol)
	{
		d2d::Vector pos = m_stage->transPosScreenToProject(x, y);
		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		sprite->translate(pos);
		m_stage->insertSprite(sprite);
	}

	return true;
}
} // coceditor