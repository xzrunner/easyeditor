#ifndef _LIBSHAPE_DRAW_POLYGON2_CMPT_H_
#define _LIBSHAPE_DRAW_POLYGON2_CMPT_H_



namespace libshape
{

class DrawPolygon2CMPT : public ee::EditCMPT
{
public:
	DrawPolygon2CMPT(wxWindow* parent, const wxString& name, wxWindow* stage_wnd,
		ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapesImpl,
		ee::PropertySettingPanel* property);

protected:
	virtual wxSizer* InitLayout();

}; // DrawPolygon2CMPT

}

#endif // _LIBSHAPE_DRAW_POLYGON2_CMPT_H_