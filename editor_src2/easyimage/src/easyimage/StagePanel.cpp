#include "StagePanel.h"
#include "StageCanvas.h"

namespace eimage
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, m_image(NULL)
{
	m_canvas = new StageCanvas(this);

	SetDropTarget(new DragSymbolTarget(this, library));
}

StagePanel::~StagePanel()
{
	if (m_image) {
		m_image->release();
	}
}

void StagePanel::setImage(const std::string& filepath)
{
	if (m_image) {
		m_image->release();
	}

	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
	if (symbol) {
		setImage(symbol);
	}
}

void StagePanel::setImage(d2d::ISymbol* symbol)
{
	d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
	d2d::Vector off;
	off.x = sprite->getSymbol().getSize().xLength() * 0.5f;
	off.y = sprite->getSymbol().getSize().yLength() * 0.5f;
	sprite->translate(off);
	m_image = sprite;

	Refresh();
}

//////////////////////////////////////////////////////////////////////////
// class StagePanel::DragSymbolTarget
//////////////////////////////////////////////////////////////////////////

StagePanel::DragSymbolTarget::
DragSymbolTarget(StagePanel* stage, d2d::LibraryPanel* library)
	: m_stage(stage)
	, m_library(library)
{
}

bool StagePanel::DragSymbolTarget::
OnDropText(wxCoord x, wxCoord y, const wxString& data)
{
	wxString sType = data.substr(0, data.find(","));
	wxString sIndex = data.substr(data.find(",") + 1);

	long index;
	sIndex.ToLong(&index);

	d2d::ISymbol* symbol = m_library->getSymbol(index);
	if (symbol) {
		m_stage->setImage(symbol);
	}

	return true;
}

}