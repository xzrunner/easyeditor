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
	CosineShape(const CosineShape& cosine) {}
	CosineShape(const std::vector<sm::vec2>& vertices);

	/**
	 *  @interface
	 *    s2::Shape
	 */
	virtual bool IsContain(const sm::vec2& pos) const { return s2::CosineShape::IsContain(pos); }
	virtual bool IsIntersect(const sm::rect& rect) const { return s2::CosineShape::IsIntersect(rect); }
	virtual void Draw(const sm::mat4& mt, const s2::RenderColor& color = s2::RenderColor()) const { s2::CosineShape::Draw(mt, color); }

	/**
	 *  @interface
	 *    ee::Shape
	 */
	virtual CosineShape* EEClone() const { return new CosineShape(*this); }
	virtual const char* GetShapeDesc() const { return "cosine-curve"; }
	virtual void Translate(const sm::vec2& offset);
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage) { return NULL; }
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir) {}
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const {}

//	const std::vector<sm::vec2>& getMidPoints() const { return m_midPoints; }

protected:
	virtual void UpdateBounding() { s2::CosineShape::UpdateBounding(); }

}; // CosineShape

}

#endif // _EASYSHAPE_COSINE_SHAPE_H_