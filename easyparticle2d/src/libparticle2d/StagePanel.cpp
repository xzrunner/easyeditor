#include "StagePanel.h"
#include "StageCanvas.h"
#include "EditOP.h"
#include "LibraryPanel.h"
#include "ParticleSystem.h"
#include "PS.h"

namespace eparticle2d
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(GetStageImpl(), library)
	, m_ps(NULL)
{
	SetCanvas(new StageCanvas(this));
	SetEditOP(new EditOP(this));
}

bool StagePanel::Update(int version)
{
	PS::Instance()->UpdateTime();
	if (m_ps) {
		return m_ps->Update(m_ps_mat);
	} else {
		return false;
	}
}

void StagePanel::SetPSMat(const d2d::Vector& pos)
{
	m_ps_mat.SetTranslation(pos.x, pos.y);
}

}