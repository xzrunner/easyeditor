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
	m_sym = new Symbol;

	SetDropTarget(new StageDropTarget(this, library));
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   wxGLContext* glctx, ee::Sprite* edited, 
					   const ee::MultiSpritesImpl* bg_sprites)
	: ee::EditPanel(parent, frame)
{
	SetCanvas(new StageCanvas(this, glctx, edited, bg_sprites));
	m_sym = dynamic_cast<eicon::Symbol*>(edited->GetSymbol());
	if (m_sym) {
		m_sym->AddReference();
	}
}

StagePanel::~StagePanel()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}
}

void StagePanel::SetIcon(Icon* icon)
{
	if (m_sym) {
		m_sym->SetIcon(icon);	
	}
}

Icon* StagePanel::GetIcon()
{
	if (m_sym) {
		return m_sym->GetIcon();
	} else {
		return NULL;
	}
}

void StagePanel::SetImage(ee::Image* img)
{
	if (m_sym) {
		m_sym->SetImage(img);
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
OnDropSymbol(ee::Symbol* sym, const sm::vec2& pos)
{
	if (ee::ImageSymbol* image = dynamic_cast<ee::ImageSymbol*>(sym))
	{
		m_stage->SetImage(image->GetImage());
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
		return true;
	}

	return false;
}

}