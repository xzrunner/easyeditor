#ifndef _EASYSHAPE_COSINE_SHAPE_H_
#define _EASYSHAPE_COSINE_SHAPE_H_

#include <ee/Shape.h>

#include <sprite2/CosineShape.h>

namespace eshape
{

class CosineShape : public ee::Shape, public s2::CosineShape
{
public:
	CosineShape() {}
	CosineShape(const CosineShape& cosine);
	CosineShape(const CU_VEC<sm::vec2>& vertices);

	/**
	 *  @interface
	 *    s2::Shape
	 */
	virtual CosineShape* Clone() const override { return new CosineShape(*this); }
	virtual bool IsContain(const sm::vec2& pos) const override { return s2::CosineShape::IsContain(pos); }
	virtual bool IsIntersect(const sm::rect& rect) const override { return s2::CosineShape::IsIntersect(rect); }
	virtual void Draw(cooking::DisplayList* dlist, const s2::RenderParams& rp) const override { s2::CosineShape::Draw(dlist, rp); }

	/**
	 *  @interface
	 *    ee::Shape
	 */
	virtual const char* GetShapeDesc() const override { return "cosine-curve"; }
	virtual void Translate(const sm::vec2& offset) override;
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage) override { return NULL; }
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir) override {}
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const override {}

//	const CU_VEC<sm::vec2>& getMidPoints() const { return m_midPoints; }

protected:
	virtual void UpdateBounding() override { s2::CosineShape::UpdateBounding(); }

}; // CosineShape

}

#endif // _EASYSHAPE_COSINE_SHAPE_H_