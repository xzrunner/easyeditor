#ifndef _EASYSHAPE_DRAW_PENCIL_POLYGON_CMPT_H_
#define _EASYSHAPE_DRAW_PENCIL_POLYGON_CMPT_H_



namespace eshape
{

class DrawPencilPolygonCMPT : public ee::OneFloatValueCMPT
{
public:
	DrawPencilPolygonCMPT(wxWindow* parent, const std::string& name, wxWindow* stage_wnd,
		ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapesImpl);

	int GetSelected() const;
	void SetSelection(const std::string& type);

protected:
	virtual wxSizer* InitLayout();

private:
	wxChoice* m_choice;

}; // DrawPencilPolygonCMPT
	
}

#endif // _EASYSHAPE_DRAW_PENCIL_POLYGON_CMPT_H_