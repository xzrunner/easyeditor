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
	ComplexPolygonShape(const std::vector<ee::Vector>& outline,
		const std::vector<std::vector<ee::Vector> >& holes);

	//
	// IObject interface
	//
	virtual ComplexPolygonShape* Clone() const { return new ComplexPolygonShape(*this); }

	//
	// Shape interface
	//
	virtual const char* GetShapeDesc() const { return "complex-polygon"; }
	virtual bool IsContain(const ee::Vector& pos) const;
	virtual bool IsIntersect(const ee::Rect& rect) const;
	virtual void Translate(const ee::Vector& offset);
	virtual void Draw(const ee::Matrix& mt,
		const ee::RenderColor& color = ee::RenderColor()) const;

	const std::vector<std::vector<ee::Vector> >& GetHoles() const { return m_holes; }

private:
	std::vector<std::vector<ee::Vector> > m_holes;

}; // ComplexPolygonShape

}

#endif // _EASYSHAPE_COMPLEX_POLYGOIN_SHAPE_H_