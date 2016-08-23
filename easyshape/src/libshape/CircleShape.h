#ifndef _EASYSHAPE_CIRCLE_SHAPE_H_
#define _EASYSHAPE_CIRCLE_SHAPE_H_

#include <ee/Shape.h>

#include <sprite2/CircleShape.h>

namespace eshape
{

class CircleShape : public ee::Shape, public s2::CircleShape
{
public:
	CircleShape() {}
	CircleShape(const CircleShape& circle);
	CircleShape(const sm::vec2& center, float radius);

	/**
	 *  @interface
	 *    s2::Shape
	 */
	virtual CircleShape* Clone() const { return new CircleShape(*this); }
	virtual bool IsContain(const sm::vec2& pos) const { return s2::CircleShape::IsContain(pos); }
	virtual bool IsIntersect(const sm::rect& rect) const { return s2::CircleShape::IsIntersect(rect); }
	virtual void Draw(const sm::mat4& mt, const s2::RenderColor& color = s2::RenderColor()) const { s2::CircleShape::Draw(mt, color); }

	/**
	 *  @interface
	 *    ee::Shape
	 */
	virtual const char* GetShapeDesc() const { return "circle"; }
	virtual void Translate(const sm::vec2& offset);
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	const sm::vec2& GetCenter() const { return m_center; }
	void SetCenter(const sm::vec2& center) { 
		m_center = center;
		UpdateBounding();
	}

	float GetRadius() const { return m_radius; }
	void SetRadius(float radius) { 
		m_radius = radius;
		UpdateBounding();
	}

protected:
	virtual void UpdateBounding() { s2::CircleShape::UpdateBounding(); }

}; // CircleShape

}

#endif // _EASYSHAPE_CIRCLE_SHAPE_H_