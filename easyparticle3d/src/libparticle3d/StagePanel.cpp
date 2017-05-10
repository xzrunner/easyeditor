#include "StagePanel.h"
#include "StageCanvas.h"
#include "EditOP.h"
#include "LibraryPanel.h"
#include "ToolBarPanel.h"
#include "ParticleSystem.h"

#include <ee/FetchAllVisitor.h>

#include <sprite2/UpdateParams.h>

namespace eparticle3d
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
	, m_ps(NULL)
{
	xRot = yRot = 0;

	SetCanvas(new StageCanvas(this));
	SetEditOP(new EditOP(this));
}

StagePanel::~StagePanel()
{
}

bool StagePanel::UpdateStage()
{
	std::vector<ee::Sprite*> sprs;
	TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprs));
	for (int i = 0, n = sprs.size(); i < n; ++i) 
	{
		s2::Sprite* child = sprs[i];
		bool old_inherit_update = child->IsInheritUpdate();
		child->SetInheritUpdate(true);

		sprs[i]->Update(s2::UpdateParams());

		child->SetInheritUpdate(old_inherit_update);
	}

	if (m_ps) {
		return m_ps->Update(m_ps_mat);
	} else {
		return false;
	}
}

void StagePanel::SetPSMat(const sm::vec2& pos)
{
	m_ps_mat.Translate(pos.x, pos.y);
}

}