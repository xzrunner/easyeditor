#ifndef _EASYSHAPE_DRAW_LINE_CMPT_H_
#define _EASYSHAPE_DRAW_LINE_CMPT_H_



namespace eshape
{

class DrawLineCMPT : public ee::EditCMPT
{
public:
	DrawLineCMPT(wxWindow* parent, const std::string& name, wxWindow* stage_wnd,
		ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapesImpl,
		ee::PropertySettingPanel* propertyPanel);

protected:
	virtual wxSizer* InitLayout();

}; // DrawLineCMPT

}

#endif // _EASYSHAPE_DRAW_LINE_CMPT_H_