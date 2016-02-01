#ifndef _EASYSHAPE_DRAW_PENCIL_LINE_CMPT_H_
#define _EASYSHAPE_DRAW_PENCIL_LINE_CMPT_H_



namespace eshape
{

class DrawPencilLineCMPT : public ee::OneFloatValueCMPT
{
public:
	DrawPencilLineCMPT(wxWindow* parent, const std::string& name, 
		wxWindow* stage_wnd, ee::EditPanelImpl* stage);

}; // DrawPencilLineCMPT
	
}

#endif // _EASYSHAPE_DRAW_PENCIL_LINE_CMPT_H_