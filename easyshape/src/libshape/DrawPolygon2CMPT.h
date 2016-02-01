#ifndef _EASYSHAPE_DRAW_POLYGON2_CMPT_H_
#define _EASYSHAPE_DRAW_POLYGON2_CMPT_H_

#include <ee/EditCMPT.h>

namespace ee { class MultiShapesImpl; class PropertySettingPanel; }

namespace eshape
{

class DrawPolygon2CMPT : public ee::EditCMPT
{
public:
	DrawPolygon2CMPT(wxWindow* parent, const std::string& name, wxWindow* stage_wnd,
		ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapes_impl,
		ee::PropertySettingPanel* property);

protected:
	virtual wxSizer* InitLayout();

}; // DrawPolygon2CMPT

}

#endif // _EASYSHAPE_DRAW_POLYGON2_CMPT_H_