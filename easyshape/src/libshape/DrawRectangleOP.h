#ifndef _EASYSHAPE_DRAW_RECTANGLE_OP_H_
#define _EASYSHAPE_DRAW_RECTANGLE_OP_H_



namespace eshape
{

class DrawRectangleOP : public ee::ZoomViewOP
{
public:
	DrawRectangleOP(wxWindow* wnd, ee::EditPanelImpl* stage, bool bOpenRightTap = true);
	
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

protected:
	ee::Vector m_firstPos, m_currPos;

	ee::ShapeStyle m_style;

}; // DrawRectangleOP

}

#endif // _EASYSHAPE_DRAW_RECTANGLE_OP_H_