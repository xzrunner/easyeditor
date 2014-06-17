#ifndef _EASYANIM_PREVIEW_OP_H_
#define _EASYANIM_PREVIEW_OP_H_

#include <drag2d.h>

namespace eanim
{

struct PlaySettings;

class PreviewOP : public d2d::ZoomViewOP
{
public:
	PreviewOP(d2d::EditPanel* editPanel, PlaySettings& settings,
		d2d::PlayControl& control);

	virtual bool onKeyDown(int keyCode);

private:
	PlaySettings& m_settings;

	d2d::PlayControl& m_control;

}; // PreviewOP

}

#endif // _EASYANIM_PREVIEW_OP_H_