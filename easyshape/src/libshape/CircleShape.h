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
	virtual CircleShape* Clone() const override { return new CircleShape(*this); }
	virtual bool IsContain(const sm::vec2& pos) const override { return s2::CircleShape::IsContain(pos); }
	virtual bool IsIntersect(const sm::rect& rect) const override { return s2::CircleShape::IsIntersect(rect); }
	virtual void Draw(cooking::DisplayList* dlist, const s2::RenderParams& rp) const override { s2::CircleShape::Draw(dlist, rp); }

	/**
	 *  @interface
	 *    ee::Shape
	 */
	virtual const char* GetShapeDesc() const override { return "circle"; }
	virtual void Translate(const sm::vec2& offset) override;
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage) override;
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir) override;
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const override;

protected:
	virtual void UpdateBounding() override { s2::CircleShape::UpdateBounding(); }

}; // CircleShape

}

#endif // _EASYSHAPE_CIRCLE_SHAPE_H_