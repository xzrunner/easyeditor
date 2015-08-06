#include "StagePanel.h"
#include "StageCanvas.h"
#include "EditOP.h"
#include "LibraryPanel.h"
#include "ToolBarPanel.h"
#include "ParticleSystem.h"

namespace eparticle3d
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, library)
	, m_ps(NULL)
	, m_last_time(-1)
{
	xRot = yRot = 0;

	SetCanvas(new StageCanvas(this));
	SetEditOP(new EditOP(this));
}

StagePanel::~StagePanel()
{
}

bool StagePanel::Update(int version)
{
	bool ret = false;

	if (m_last_time == -1) {
		m_last_time = clock();
	} else {
		clock_t curr = clock();
		int dt = curr - m_last_time;
		m_last_time = curr;

		if (m_ps) {
			m_ps->update((float)dt / CLOCKS_PER_SEC);
			ret = true;
		}
	}

	return ret;
}

}