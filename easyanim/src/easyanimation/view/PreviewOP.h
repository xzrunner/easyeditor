#ifndef _EASYANIM_PREVIEW_OP_H_
#define _EASYANIM_PREVIEW_OP_H_

#include <drag2d.h>

namespace eanim
{

struct PlaySettings;

class PreviewOP : public d2d::ZoomViewOP
{
public:
	PreviewOP(wxWindow* wnd, d2d::EditPanelImpl* stage, 
		PlaySettings& settings, d2d::PlayControl& control);

	virtual bool OnKeyDown(int keyCode);

private:
	PlaySettings& m_settings;

	d2d::PlayControl& m_control;

}; // PreviewOP

}

#endif // _EASYANIM_PREVIEW_OP_H_