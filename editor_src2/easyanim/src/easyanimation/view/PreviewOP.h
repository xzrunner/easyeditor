#ifndef _EASYANIM_PREVIEW_OP_H_
#define _EASYANIM_PREVIEW_OP_H_

#include <drag2d.h>

namespace eanim
{

struct PlaySettings;
class PreviewControl;

class PreviewOP : public d2d::ZoomViewOP
{
public:
	PreviewOP(d2d::EditPanel* editPanel, PlaySettings& settings,
		PreviewControl& control);

	virtual bool onKeyDown(int keyCode);

private:
	PlaySettings& m_settings;

	PreviewControl& m_control;

}; // PreviewOP

}

#endif // _EASYANIM_PREVIEW_OP_H_