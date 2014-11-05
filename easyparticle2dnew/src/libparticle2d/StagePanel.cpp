#include "StagePanel.h"
#include "StageCanvas.h"
#include "EditOP.h"
#include "LibraryPanel.h"
#include "ToolBarPanel.h"
#include "ParticleSystem.h"

namespace eparticle2d
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, library)
	, m_ps(NULL)
{
	m_canvas = new StageCanvas(this);
//	m_editOP = new EditOP(this);

	d2d::ArrangeSpriteConfig cfg;
	cfg.is_auto_align_open = false;
	cfg.is_deform_open = false;
	cfg.is_offset_open = false;
	cfg.is_rotate_open = false;
	m_editOP = new d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(this, this, NULL, NULL, cfg);

	m_ps = new ParticleSystem(1000);
}

StagePanel::~StagePanel()
{
}

}