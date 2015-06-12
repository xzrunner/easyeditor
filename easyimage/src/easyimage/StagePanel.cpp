#include "StagePanel.h"
#include "StageCanvas.h"

namespace eimage
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, m_image(NULL)
{
	m_left = m_right = NULL;

	m_canvas = new StageCanvas(this);

	SetDropTarget(new StageDropTarget(this, library));
}

StagePanel::~StagePanel()
{
	if (m_image) {
		m_image->Release();
	}
}

void StagePanel::setImage(const std::string& filepath)
{
	if (m_image) {
		m_image->Release();
	}

	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
	if (symbol) {
		setImage(symbol);
		symbol->Release();
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
// class StagePanel::StageDropTarget
//////////////////////////////////////////////////////////////////////////

StagePanel::StageDropTarget::
StageDropTarget(StagePanel* stage, d2d::LibraryPanel* library)
	: d2d::CombinedDropTarget(stage)
	, m_stage(stage)
	, m_library(library)
{
}

void StagePanel::StageDropTarget::
OnDropText(wxCoord x, wxCoord y, const wxString& text)
{
	wxString sType = text.substr(0, text.find(","));
	wxString sIndex = text.substr(text.find(",") + 1);

	long index;
	sIndex.ToLong(&index);

	d2d::ISymbol* symbol = m_library->GetSymbol(index);
	if (symbol) {
		m_stage->setImage(symbol);
	}

// 	// todo for diff
// 	// fixme
// 	d2d::Vector pos = m_stage->transPosScreenToProject(x, y);
// 	d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
// 	d2d::Rect r = sprite->getSymbol().getSize();
// 	if (pos.x < 0) {
// 		sprite->setTransform(d2d::Vector(-r.xLength() * 0.5f - 10, 0.0f), 0);
// 		m_stage->m_left = sprite;
// 	} else {
// 		sprite->setTransform(d2d::Vector(r.xLength() * 0.5f + 10, 0.0f), 0);
// 		m_stage->m_right = sprite;
// 	}
}

void StagePanel::StageDropTarget::
OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
	if (filenames.IsEmpty()) {
		return;
	}

	wxString filename = filenames[0];
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filename);
	m_stage->setImage(symbol);
	symbol->Release();
}

}