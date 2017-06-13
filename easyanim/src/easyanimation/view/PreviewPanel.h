#ifndef _EASYANIM_PREVIEW_PANEL_H_
#define _EASYANIM_PREVIEW_PANEL_H_

#include <ee/EditPanel.h>

#include <sprite2/s2_config.h>

namespace s2 { class AnimTreeCurr; }

namespace eanim
{

class PreviewPanel : public ee::EditPanel
{
public:
	PreviewPanel(wxWindow* parent, wxTopLevelWindow* frame, s2::AnimTreeCurr& curr);

	//
	// ee::EditPanel interface
	//
	virtual bool UpdateStage();

	void SetLoop(bool loop) { m_loop = loop; }

private:
	s2::AnimTreeCurr& m_curr;

	bool m_loop;

	int m_fps;

}; // PreviewPanel

}

#endif // _EASYANIM_PREVIEW_PANEL_H_