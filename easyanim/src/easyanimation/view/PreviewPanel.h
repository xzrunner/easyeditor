#ifndef _EASYANIM_PREVIEW_PANEL_H_
#define _EASYANIM_PREVIEW_PANEL_H_

#include <ee/EditPanel.h>

namespace ee { class PlayControl; }

namespace eanim
{

class PreviewPanel : public ee::EditPanel
{
public:
	PreviewPanel(wxWindow* parent, wxTopLevelWindow* frame,
		ee::PlayControl& ctrl);

	//
	// ee::EditPanel interface
	//
	virtual bool UpdateStage();

private:
	ee::PlayControl& m_ctrl;

}; // PreviewPanel

}

#endif // _EASYANIM_PREVIEW_PANEL_H_