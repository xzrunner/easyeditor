#include "StagePanel.h"
#include "StageCanvas.h"
#include "SelectSpritesOP.h"
#include "LibraryPanel.h"
#include "ParticleSystem.h"

namespace eparticle2d
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(GetStageImpl(), library)
	, m_ps(NULL)
	, m_last_time(-1)
{
	SetCanvas(new StageCanvas(this));

	d2d::ArrangeSpriteConfig cfg;
	cfg.is_auto_align_open = false;
	cfg.is_deform_open = false;
	cfg.is_offset_open = false;
	cfg.is_rotate_open = false;

	SetEditOP(new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, GetStageImpl(), this, NULL, NULL, NULL, cfg));
}

bool StagePanel::Update(int version)
{
	bool ret = false;

	if (!m_ps) {
		return ret;
	}

	if (m_last_time == -1) {
		m_last_time = clock();
	} else {
		clock_t curr = clock();
		int dt = curr - m_last_time;
		m_last_time = curr;

		if (m_ps) {
			m_ps->Update((float)dt / CLOCKS_PER_SEC);
			ret = true;
		}
	}

	return ret;
}

void StagePanel::SetSelectedSymbol(Symbol* symbol)
{
//	m_data.SetSelected(symbol);
}

void StagePanel::SetToolbar(ToolbarPanel* toolbar)
{
//	m_data.SetToolbar(toolbar);
}

}