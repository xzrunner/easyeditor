#ifndef _EASYSHAPE_DRAW_CURVE_OP_H_
#define _EASYSHAPE_DRAW_CURVE_OP_H_



namespace eshape
{

class DrawCurveOP : public ee::ZoomViewOP
{
public:
	DrawCurveOP(wxWindow* wnd, ee::EditPanelImpl* stage);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

protected:
	std::vector<ee::Vector> m_curve;

private:
	ee::Vector m_firstPos;
	bool m_startDraw;

	bool m_straight_mode;

}; // DrawCurveOP

}

#endif // _EASYSHAPE_DRAW_CURVE_OP_H_