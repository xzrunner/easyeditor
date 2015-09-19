#include "StagePanel.h"
#include "StageCanvas.h"
#include "Symbol.h"

namespace eicon
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
{
	SetCanvas(new StageCanvas(this));
	m_symbol = new Symbol;

	SetDropTarget(new StageDropTarget(this, library));
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::ISprite* edited, const d2d::MultiSpritesImpl* bg_sprites)
	: d2d::EditPanel(parent, frame)
{
	SetCanvas(new StageCanvas(this, edited, bg_sprites));
	m_symbol = (Symbol*)(&edited->GetSymbol());
	if (m_symbol) {
		m_symbol->Retain();
	}
}

StagePanel::~StagePanel()
{
	if (m_symbol) {
		m_symbol->Release();
	}
}

void StagePanel::Clear()
{
	EditPanel::Clear();
}

void StagePanel::SetIcon(Icon* icon)
{
	if (m_symbol) {
		m_symbol->SetIcon(icon);	
	}
}

Icon* StagePanel::GetIcon()
{
	if (m_symbol) {
		return m_symbol->GetIcon();
	} else {
		return NULL;
	}
}

void StagePanel::SetImage(d2d::Image* img)
{
	if (m_symbol) {
		m_symbol->SetImage(img);
	}
}

//////////////////////////////////////////////////////////////////////////
// class StagePanel::StageDropTarget
//////////////////////////////////////////////////////////////////////////

StagePanel::StageDropTarget::
StageDropTarget(StagePanel* stage, d2d::LibraryPanel* library)
	: d2d::StageDropTarget(stage, stage->GetStageImpl(), NULL, library)
	, m_stage(stage)
{
}

bool StagePanel::StageDropTarget::
OnDropSymbol(d2d::ISymbol* symbol, const d2d::Vector& pos)
{
	if (d2d::ImageSymbol* image = dynamic_cast<d2d::ImageSymbol*>(symbol))
	{
		m_stage->SetImage(image->getImage());
		m_stage->SetCanvasDirty();
		return true;
	}

	return false;
}

}