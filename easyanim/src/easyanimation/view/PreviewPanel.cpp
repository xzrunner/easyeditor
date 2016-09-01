#include "PreviewPanel.h"
#include "PreviewUtility.h"

#include <ee/Sprite.h>

#include <sprite2/RenderParams.h>
#include <sprite2/Particle3d.h>

namespace eanim
{

PreviewPanel::PreviewPanel(wxWindow* parent, wxTopLevelWindow* frame,
						   ee::PlayControl& ctrl)
	: ee::EditPanel(parent, frame)
	, m_ctrl(ctrl)
{
}

bool PreviewPanel::Update(float dt)
{
	std::vector<ee::Sprite*> sprs;
	PreviewUtility::GetCurrSprites(m_ctrl, sprs);

	s2::RenderParams params;
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		sprs[i]->Update(params);
	}

	for (size_t i = 0, n = sprs.size(); i < n; ++i) {
		sprs[i]->RemoveReference();
	}

	return true;
}

}