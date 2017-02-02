#ifndef _EASYANIM_PREVIEW_PANEL_H_
#define _EASYANIM_PREVIEW_PANEL_H_

#include <ee/EditPanel.h>

#ifdef S2_ANIM_CURR_OLD
namespace s2 { class AnimCurr; }
#else
namespace s2 { class AnimCurr2; }
#endif // S2_ANIM_CURR_OLD

namespace eanim
{

class PreviewPanel : public ee::EditPanel
{
public:
#ifdef S2_ANIM_CURR_OLD
	PreviewPanel(wxWindow* parent, wxTopLevelWindow* frame, s2::AnimCurr& curr);
#else
	PreviewPanel(wxWindow* parent, wxTopLevelWindow* frame, s2::AnimCurr2& curr);
#endif // S2_ANIM_CURR_OLD

	//
	// ee::EditPanel interface
	//
	virtual bool UpdateStage();

	void SetLoop(bool loop) { m_loop = loop; }

private:
#ifdef S2_ANIM_CURR_OLD
	s2::AnimCurr& m_curr;
#else
	s2::AnimCurr2& m_curr;
#endif // S2_ANIM_CURR_OLD

	bool m_loop;

	int m_fps;

}; // PreviewPanel

}

#endif // _EASYANIM_PREVIEW_PANEL_H_