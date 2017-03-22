#ifndef _EASYANIM_PREVIEW_PANEL_H_
#define _EASYANIM_PREVIEW_PANEL_H_

#include <ee/EditPanel.h>

#include <sprite2/s2_config.h>

namespace s2 { class AnimCurr; }

namespace eanim
{

class PreviewPanel : public ee::EditPanel
{
public:
	PreviewPanel(wxWindow* parent, wxTopLevelWindow* frame, s2::AnimCurr& curr);

	//
	// ee::EditPanel interface
	//
	virtual bool UpdateStage();

	void SetLoop(bool loop) { m_loop = loop; }

private:
	s2::AnimCurr& m_curr;

	bool m_loop;

	int m_fps;

}; // PreviewPanel

}

#endif // _EASYANIM_PREVIEW_PANEL_H_