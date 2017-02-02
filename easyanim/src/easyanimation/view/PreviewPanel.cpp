#include "PreviewPanel.h"

#include "message/messages.h"

#include <ee/Sprite.h>

#include <sprite2/RenderParams.h>
#include <sprite2/Particle3d.h>
#ifdef S2_ANIM_CURR_OLD
#include <sprite2/AnimCurr.h>
#else
#include <sprite2/AnimCurr2.h>
#endif // S2_ANIM_CURR_OLD

namespace eanim
{

#ifdef S2_ANIM_CURR_OLD
PreviewPanel::PreviewPanel(wxWindow* parent, wxTopLevelWindow* frame, s2::AnimCurr& curr)
#else
PreviewPanel::PreviewPanel(wxWindow* parent, wxTopLevelWindow* frame, s2::AnimCurr2& curr)
#endif // S2_ANIM_CURR_OLD
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