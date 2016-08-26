#include "StagePanel.h"
#include "StageCanvas.h"
#include "EditOP.h"
#include "LibraryPanel.h"
#include "ParticleSystem.h"
#include "PSNode.h"

namespace eparticle2d
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
	, m_ps(NULL)
{
	SetCanvas(new StageCanvas(this));
	SetEditOP(new EditOP(this));
}

bool StagePanel::Update(float dt)
{
	PSNode::Instance()->UpdateTime();
	if (m_ps) {
		return m_ps->Update(m_ps_mat);
	} else {
		return false;
	}
}

void StagePanel::SetPSMat(const sm::vec2& pos)
{
	m_ps_mat = sm::mat4::Translated(pos.x, pos.y, 0);
}

}