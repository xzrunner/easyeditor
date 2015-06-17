#ifndef _LIBSHAPE_DRAW_POLYGON2_CMPT_H_
#define _LIBSHAPE_DRAW_POLYGON2_CMPT_H_

#include <drag2d.h>

namespace libshape
{

class DrawPolygon2CMPT : public d2d::AbstractEditCMPT
{
public:
	DrawPolygon2CMPT(wxWindow* parent, const wxString& name,
		d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
		d2d::PropertySettingPanel* property,
		d2d::ViewPanelMgr* view_panel_mgr);

protected:
	virtual wxSizer* initLayout();

}; // DrawPolygon2CMPT

}

#endif // _LIBSHAPE_DRAW_POLYGON2_CMPT_H_