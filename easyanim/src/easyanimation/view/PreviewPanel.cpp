#include "PreviewPanel.h"

#include "message/messages.h"

#include <ee/Sprite.h>

#include <sprite2/RenderParams.h>
#include <sprite2/Particle3d.h>
#ifdef S2_ANIM_CURR_V0
#include <sprite2/AnimCurr.h>
#elif defined S2_ANIM_CURR_V1
#include <sprite2/AnimCurr2.h>
#elif defined S2_ANIM_CURR_V2
#include <sprite2/AnimCurr3.h>
#endif

namespace eanim
{

#ifdef S2_ANIM_CURR_V0
PreviewPanel::PreviewPanel(wxWindow* parent, wxTopLevelWindow* frame, s2::AnimCurr& curr)
#elif defined S2_ANIM_CURR_V1
PreviewPanel::PreviewPanel(wxWindow* parent, wxTopLevelWindow* frame, s2::AnimCurr2& curr)
#elif defined S2_ANIM_CURR_V2
PreviewPanel::PreviewPanel(wxWindow* parent, wxTopLevelWindow* frame, s2::AnimCurr3& curr)
#endif
	: ee::EditPanel(parent, frame)
	, m_curr(curr)
	, m_loop(true)
{
	m_fps = GetFpsSJ::Instance()->Get();
}

bool PreviewPanel::UpdateStage()
{
	return m_curr.Update(s2::RenderParams(), m_loop, 0, m_fps);
}

}