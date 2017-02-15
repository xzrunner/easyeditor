#ifndef _EASYANIM_PREVIEW_PANEL_H_
#define _EASYANIM_PREVIEW_PANEL_H_

#include <ee/EditPanel.h>

#include <sprite2/s2_config.h>

#ifdef S2_ANIM_CURR_V0
namespace s2 { class AnimCurr; }
#elif defined S2_ANIM_CURR_V1
namespace s2 { class AnimCurr2; }
#elif defined S2_ANIM_CURR_V2
namespace s2 { class AnimCurr3; }
#endif

namespace eanim
{

class PreviewPanel : public ee::EditPanel
{
public:
#ifdef S2_ANIM_CURR_V0
	PreviewPanel(wxWindow* parent, wxTopLevelWindow* frame, s2::AnimCurr& curr);
#elif defined S2_ANIM_CURR_V1
	PreviewPanel(wxWindow* parent, wxTopLevelWindow* frame, s2::AnimCurr2& curr);
#elif defined S2_ANIM_CURR_V2
	PreviewPanel(wxWindow* parent, wxTopLevelWindow* frame, s2::AnimCurr3& curr);
#endif

	//
	// ee::EditPanel interface
	//
	virtual bool UpdateStage();

	void SetLoop(bool loop) { m_loop = loop; }

private:
#ifdef S2_ANIM_CURR_V0
	s2::AnimCurr& m_curr;
#elif defined S2_ANIM_CURR_V1
	s2::AnimCurr2& m_curr;
#elif defined S2_ANIM_CURR_V2
	s2::AnimCurr3& m_curr;
#endif

	bool m_loop;

	int m_fps;

}; // PreviewPanel

}

#endif // _EASYANIM_PREVIEW_PANEL_H_