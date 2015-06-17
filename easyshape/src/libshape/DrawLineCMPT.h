#ifndef _LIBSHAPE_DRAW_LINE_CMPT_H_
#define _LIBSHAPE_DRAW_LINE_CMPT_H_

#include <drag2d.h>

namespace libshape
{

class DrawLineCMPT : public d2d::AbstractEditCMPT
{
public:
	DrawLineCMPT(wxWindow* parent, const wxString& name,
		d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
		d2d::PropertySettingPanel* propertyPanel, d2d::ViewPanelMgr* view_panel_mgr);

protected:
	virtual wxSizer* initLayout();

}; // DrawLineCMPT

}

#endif // _LIBSHAPE_DRAW_LINE_CMPT_H_