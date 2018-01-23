#include "StagePanel.h"
#include "StageCanvas.h"
#include "Symbol.h"
#include "Icon.h"

#include <ee/Sprite.h>
#include <ee/ImageSymbol.h>
#include <ee/panel_msg.h>

namespace eicon
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
{
	SetCanvas(std::make_shared<StageCanvas>(this));
	m_sym = std::make_shared<Symbol>();

	SetDropTarget(new StageDropTarget(this, library));
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   wxGLContext* glctx, const ee::SprPtr& edited, 
					   const ee::MultiSpritesImpl* bg_sprites)
	: ee::EditPanel(parent, frame)
{
	SetCanvas(std::make_shared<StageCanvas>(this, glctx, edited, bg_sprites));
}

void StagePanel::SetIcon(std::unique_ptr<s2::Icon>& icon)
{
	if (m_sym) {
		m_sym->SetIcon(icon);
	}
}

Icon* StagePanel::GetIcon()
{
	if (m_sym) {
		return dynamic_cast<Icon*>(m_sym->GetIcon().get());
	} else {
		return nullptr;
	}
}

void StagePanel::SetImage(const std::shared_ptr<ee::ImageSymbol>& img)
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
OnDropSymbol(const ee::SymPtr& sym, const sm::vec2& pos)
{
	if (auto image = std::dynamic_pointer_cast<ee::ImageSymbol>(sym))
	{
		m_stage->SetImage(image);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
		return true;
	}

	return false;
}

}