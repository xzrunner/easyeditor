#ifndef _EASYSHAPE_DRAW_PENCIL_POLYGON_OP_H_
#define _EASYSHAPE_DRAW_PENCIL_POLYGON_OP_H_

#include "DrawCurveOP.h"

namespace ee { class MultiShapesImpl; class OneFloatValue; }

namespace eshape
{

class DrawPencilPolygonCMPT;

class DrawPencilPolygonOP : public DrawCurveOP
{
public:
	DrawPencilPolygonOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapes_impl,
		ee::OneFloatValue* simplify, DrawPencilPolygonCMPT* cmpt);

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftUp(int x, int y);

private:
	void NewPolygon(const std::vector<ee::Vector>& poly);

	void UnionPolygon(const std::vector<ee::Vector>& poly);
	void DifferencePolygon(const std::vector<ee::Vector>& poly);
	void IntersectionPolygon(const std::vector<ee::Vector>& poly);
	void XorPolygon(const std::vector<ee::Vector>& poly);

	void PrepareSubjectPaths(std::vector<std::vector<ee::Vector> >& paths) const;
	void ReplacePolygons(const std::vector<std::vector<ee::Vector> >& paths);

private:
	enum Type
	{
		e_normal = 0,
		e_union,
		e_difference,
		e_intersection,
		e_xor
	};

private:
	ee::MultiShapesImpl* m_shapes_impl;

	ee::OneFloatValue* m_simplify;

	DrawPencilPolygonCMPT* m_cmpt;

}; // DrawPencilPolygonOP

}

#endif // _EASYSHAPE_DRAW_PENCIL_POLYGON_OP_H_