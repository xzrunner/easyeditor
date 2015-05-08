#ifndef _LIBSHAPE_COMPLEX_POLYGOIN_SHAPE_H_
#define _LIBSHAPE_COMPLEX_POLYGOIN_SHAPE_H_

#include "PolygonShape.h"

namespace libshape
{

class Material;

class ComplexPolygonShape : public PolygonShape
{
public:
	ComplexPolygonShape();
	ComplexPolygonShape(const ComplexPolygonShape& poly);
	ComplexPolygonShape(const std::vector<d2d::Vector>& outline,
		const std::vector<std::vector<d2d::Vector> >& holes);

	//
	// IObject interface
	//
	virtual ComplexPolygonShape* clone() const { return new ComplexPolygonShape(*this); }

	//
	// IShape interface
	//
	virtual const char* GetShapeDesc() const { return "complex-polygon"; }
	virtual bool isContain(const d2d::Vector& pos) const;
	virtual bool isIntersect(const d2d::Rect& rect) const;
	virtual void Translate(const d2d::Vector& offset);
	virtual void draw(const d2d::Matrix& mt,
		const d2d::Colorf& color = d2d::Colorf(0, 0, 0)) const;

	const std::vector<std::vector<d2d::Vector> >& GetHoles() const { return m_holes; }

private:
	std::vector<std::vector<d2d::Vector> > m_holes;

}; // ComplexPolygonShape

}

#endif // _LIBSHAPE_COMPLEX_POLYGOIN_SHAPE_H_