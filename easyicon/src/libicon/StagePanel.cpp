#include "StagePanel.h"
#include "StageCanvas.h"
#include "Symbol.h"

#include <ee/Sprite.h>
#include <ee/ImageSymbol.h>
#include <ee/panel_msg.h>

namespace eicon
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
{
	SetCanvas(new StageCanvas(this));
	m_symbol = new Symbol;

	SetDropTarget(new StageDropTarget(this, library));
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   wxGLContext* glctx, ee::Sprite* edited, 
					   const ee::MultiSpritesImpl* bg_sprites)
	: ee::EditPanel(parent, frame)
{
	SetCanvas(new StageCanvas(this, glctx, edited, bg_sprites));
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

void StagePanel::SetImage(ee::Image* img)
{
	if (m_symbol) {
		m_symbol->SetImage(img);
	}
}

//////////////////////////////////////////////////////////////////////////
// class StagePanel::StageDropTarget
//////////////////////////////////////////////////////////////////////////

StagePanel::StageDropTarget::
StageDropTarget(StagePanel* stage, ee::LibraryPanel* library)
	: ee::StageDropTarget(stage, stage->GetStageImpl(), library)
	, m_stage(stage)
{
}

bool StagePanel::StageDropTarget::
OnDropSymbol(ee::Symbol* symbol, const sm::vec2& pos)
{
	if (ee::ImageSymbol* image = dynamic_cast<ee::ImageSymbol*>(symbol))
	{
		m_stage->SetImage(image->GetImage());
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
		return true;
	}

	return false;
}

}