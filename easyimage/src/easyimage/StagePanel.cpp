#include "StagePanel.h"
#include "StageCanvas.h"

#include <ee/Sprite.h>
#include <ee/SymbolMgr.h>
#include <ee/EditOP.h>
#include <ee/Symbol.h>
#include <ee/LibraryPanel.h>
#include <ee/SpriteFactory.h>
#include <ee/panel_msg.h>
#include <ee/ImageSymbol.h>
#include <ee/Image.h>

namespace eimage
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, m_image(NULL)
{
	m_left = m_right = NULL;

	SetCanvas(new StageCanvas(this));
	SetDropTarget(new StageDropTarget(this, library));
}

StagePanel::~StagePanel()
{
	if (m_image) {
		m_image->RemoveReference();
	}
}

void StagePanel::SetImage(const std::string& filepath)
{
	if (m_image) {
		m_image->RemoveReference();
	}

	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	if (sym) {
		SetImage(sym);
		sym->RemoveReference();
	}
}

void StagePanel::SetImage(ee::Symbol* sym)
{
	ee::ImageSymbol* img_sym = static_cast<ee::ImageSymbol*>(sym);
	sm::vec2 offset = - img_sym->GetImage()->GetOffset();

	ee::Sprite* spr = ee::SpriteFactory::Instance()->Create(sym);
	offset += spr->GetSymbol()->GetBounding().Size() * 0.5f;
	spr->Translate(offset);
	m_image = spr;

	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	GetEditOP()->Clear();
}

//////////////////////////////////////////////////////////////////////////
// class StagePanel::StageDropTarget
//////////////////////////////////////////////////////////////////////////

StagePanel::StageDropTarget::
StageDropTarget(StagePanel* stage, ee::LibraryPanel* library)
	: ee::CombinedDropTarget(stage)
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

	ee::Symbol* sym = m_library->GetSymbol(index);
	if (sym) {
		m_stage->SetImage(sym);
	}

// 	// todo for diff
// 	// fixme
// 	sm::vec2 pos = m_stage->transPosScreenToProject(x, y);
// 	ee::Sprite* spr = ee::SpriteFactory::Instance()->Create(sym);
// 	sm::rect r = spr->getSymbol().getSize();
// 	if (pos.x < 0) {
// 		spr->setTransform(sm::vec2(-r.Width() * 0.5f - 10, 0.0f), 0);
// 		m_stage->m_left = spr;
// 	} else {
// 		spr->setTransform(sm::vec2(r.Width() * 0.5f + 10, 0.0f), 0);
// 		m_stage->m_right = spr;
// 	}
}

void StagePanel::StageDropTarget::
OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
	if (filenames.IsEmpty()) {
		return;
	}

	std::string filename = filenames[0];
	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filename);
	m_stage->SetImage(sym);
	sym->RemoveReference();
}

}