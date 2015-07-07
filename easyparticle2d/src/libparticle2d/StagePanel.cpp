#include "StagePanel.h"
#include "StageCanvas.h"
#include "SelectSpritesOP.h"
#include "LibraryPanel.h"

namespace eparticle2d
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, library)
{
	m_canvas = new StageCanvas(this);

	d2d::ArrangeSpriteConfig cfg;
	cfg.is_auto_align_open = false;
	cfg.is_deform_open = false;
	cfg.is_offset_open = false;
	cfg.is_rotate_open = false;
	m_edit_op = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, this, NULL, NULL, NULL, cfg);
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