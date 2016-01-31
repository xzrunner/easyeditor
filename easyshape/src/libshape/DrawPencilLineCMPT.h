#ifndef _LIBSHAPE_DRAW_PENCIL_LINE_CMPT_H_
#define _LIBSHAPE_DRAW_PENCIL_LINE_CMPT_H_



namespace libshape
{

class DrawPencilLineCMPT : public ee::OneFloatValueCMPT
{
public:
	DrawPencilLineCMPT(wxWindow* parent, const wxString& name, 
		wxWindow* stage_wnd, ee::EditPanelImpl* stage);

}; // DrawPencilLineCMPT
	
}

#endif // _LIBSHAPE_DRAW_PENCIL_LINE_CMPT_H_