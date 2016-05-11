#ifndef _EASYSHAPE_COMPLEX_POLYGOIN_SHAPE_H_
#define _EASYSHAPE_COMPLEX_POLYGOIN_SHAPE_H_

#include "PolygonShape.h"

namespace eshape
{

class Material;

class ComplexPolygonShape : public PolygonShape
{
public:
	ComplexPolygonShape();
	ComplexPolygonShape(const ComplexPolygonShape& poly);
	ComplexPolygonShape(const std::vector<sm::vec2>& outline,
		const std::vector<std::vector<sm::vec2> >& holes);

	//
	// IObject interface
	//
	virtual ComplexPolygonShape* Clone() const { return new ComplexPolygonShape(*this); }

	//
	// Shape interface
	//
	virtual const char* GetShapeDesc() const { return "complex-polygon"; }
	virtual bool IsContain(const sm::vec2& pos) const;
	virtual bool IsIntersect(const ee::Rect& rect) const;
	virtual void Translate(const sm::vec2& offset);
	virtual void Draw(const sm::mat4& mt,
		const s2::RenderColor& color = s2::RenderColor()) const;

	const std::vector<std::vector<sm::vec2> >& GetHoles() const { return m_holes; }

private:
	std::vector<std::vector<sm::vec2> > m_holes;

}; // ComplexPolygonShape

}

#endif // _EASYSHAPE_COMPLEX_POLYGOIN_SHAPE_H_