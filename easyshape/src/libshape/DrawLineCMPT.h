#ifndef _LIBSHAPE_DRAW_LINE_CMPT_H_
#define _LIBSHAPE_DRAW_LINE_CMPT_H_



namespace libshape
{

class DrawLineCMPT : public ee::EditCMPT
{
public:
	DrawLineCMPT(wxWindow* parent, const wxString& name, wxWindow* stage_wnd,
		ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapesImpl,
		ee::PropertySettingPanel* propertyPanel);

protected:
	virtual wxSizer* InitLayout();

}; // DrawLineCMPT

}

#endif // _LIBSHAPE_DRAW_LINE_CMPT_H_