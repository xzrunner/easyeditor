//#ifndef _EASYSHAPE_COMPLEX_POLYGOIN_SHAPE_H_
//#define _EASYSHAPE_COMPLEX_POLYGOIN_SHAPE_H_
//
//#include "PolygonShape.h"
//
//namespace eshape
//{
//
//class Material;
//
//class ComplexPolygonShape : public PolygonShape
//{
//public:
//	ComplexPolygonShape();
//	ComplexPolygonShape(const ComplexPolygonShape& poly);
//	ComplexPolygonShape(const CU_VEC<sm::vec2>& outline,
//		const CU_VEC<CU_VEC<sm::vec2> >& holes);
//
//	//
//	// IObject interface
//	//
//	virtual ComplexPolygonShape* Clone() const { return new ComplexPolygonShape(*this); }
//
//	//
//	// Shape interface
//	//
//	virtual const char* GetShapeDesc() const override { return "complex-polygon"; }
//	virtual bool IsContain(const sm::vec2& pos) const override;
//	virtual bool IsIntersect(const sm::rect& rect) const override;
//	virtual void Translate(const sm::vec2& offset) override;
//	virtual void Draw(const S2_MAT& mt,
//		const s2::RenderColor& color = s2::RenderColor()) const;
//
//	const CU_VEC<CU_VEC<sm::vec2> >& GetHoles() const { return m_holes; }
//
//private:
//	CU_VEC<CU_VEC<sm::vec2> > m_holes;
//
//}; // ComplexPolygonShape
//
//}
//
//#endif // _EASYSHAPE_COMPLEX_POLYGOIN_SHAPE_H_