#include "StagePanel.h"
#include "StageCanvas.h"
#include "SelectSpritesOP.h"
#include "LibraryPanel.h"

namespace eparticle2d
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(GetStageImpl(), library)
{
	SetCanvas(new StageCanvas(this));

	d2d::ArrangeSpriteConfig cfg;
	cfg.is_auto_align_open = false;
	cfg.is_deform_open = false;
	cfg.is_offset_open = false;
	cfg.is_rotate_open = false;

	SetEditOP(new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, GetStageImpl(), this, NULL, NULL, NULL, cfg));
}

void StagePanel::SetSelectedSymbol(Symbol* symbol)
{
	m_data.SetSelected(symbol);
}

void StagePanel::SetToolbar(ToolbarPanel* toolbar)
{
	m_data.SetToolbar(toolbar);
}

}