#ifndef _EASYANIM_PREVIEW_PANEL_H_
#define _EASYANIM_PREVIEW_PANEL_H_

#include <drag2d.h>

namespace eanim
{

class PreviewPanel : public d2d::EditPanel
{
public:
	PreviewPanel(wxWindow* parent, wxTopLevelWindow* frame,
		d2d::PlayControl& ctrl);

	//
	// d2d::EditPanel interface
	//
	virtual bool Update(int version);

private:
	d2d::PlayControl& m_ctrl;

}; // PreviewPanel

}

#endif // _EASYANIM_PREVIEW_PANEL_H_