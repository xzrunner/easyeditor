#ifndef _EASYSHAPE_DRAW_LINE_CMPT_H_
#define _EASYSHAPE_DRAW_LINE_CMPT_H_

#include "ee/EditCMPT.h"

namespace ee { class EditPanelImpl; class MultiShapesImpl; class PropertySettingPanel; }
 
namespace eshape
{

class DrawLineCMPT : public ee::EditCMPT
{
public:
	DrawLineCMPT(wxWindow* parent, const std::string& name, wxWindow* stage_wnd,
		ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapes_impl,
		ee::PropertySettingPanel* property);

protected:
	virtual wxSizer* InitLayout();

}; // DrawLineCMPT

}

#endif // _EASYSHAPE_DRAW_LINE_CMPT_H_