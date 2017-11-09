#include "PreviewPanel.h"

#include "message/messages.h"

#include <ee/Sprite.h>

#include <sprite2/UpdateParams.h>
#include <sprite2/Particle3d.h>
#include <sprite2/AnimCurr.h>

namespace eanim
{

PreviewPanel::PreviewPanel(wxWindow* parent, wxTopLevelWindow* frame, s2::AnimCurr& curr)
	: ee::EditPanel(parent, frame)
	, m_curr(curr)
	, m_loop(true)
{
	m_fps = GetFpsSJ::Instance()->Get();
}

bool PreviewPanel::UpdateStage()
{
	s2::UpdateParams params;
	params.SetForce(true);
	return m_curr.Update(params, NULL, NULL, m_loop, 0, m_fps);
}

}