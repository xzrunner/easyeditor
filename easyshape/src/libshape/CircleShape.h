#ifndef _EASYSHAPE_CIRCLE_SHAPE_H_
#define _EASYSHAPE_CIRCLE_SHAPE_H_

#include <ee/Shape.h>

#include <sprite2/CircleShape.h>

namespace eshape
{

class CircleShape : public ee::Shape
{
public:
	CircleShape() {}
	CircleShape(const sm::vec2& center, float radius);

	//
	// IObject interface
	//
	virtual CircleShape* Clone() const;

	//
	// Shape interface
	//
	virtual const char* GetShapeDesc() const { return "circle"; }
	virtual bool IsContain(const sm::vec2& pos) const { return m_core.IsContain(pos); }
	virtual bool IsIntersect(const sm::rect& rect) const { return m_core.IsIntersect(rect); }
	virtual void Translate(const sm::vec2& offset);
	virtual const sm::rect& GetRect() const { return m_core.GetRegion(); }
	virtual void Draw(const sm::mat4& mt,
		const s2::RenderColor& color = s2::RenderColor()) const { m_core.Draw(mt, color); }
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	const sm::vec2& GetCenter() const { return m_core.GetCenter(); }
	void SetCenter(const sm::vec2& center) { m_core.SetCenter(center); }

	float GetRadius() const { return m_core.GetRadius(); }
	void SetRadius(float radius) { m_core.SetRaidius(radius); }

private:
	s2::CircleShape m_core;

}; // CircleShape

}

#endif // _EASYSHAPE_CIRCLE_SHAPE_H_