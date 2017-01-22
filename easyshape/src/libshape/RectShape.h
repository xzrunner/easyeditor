#ifndef _EASYSHAPE_RECT_SHAPE_H_
#define _EASYSHAPE_RECT_SHAPE_H_

#include <ee/Shape.h>

#include <sprite2/RectShape.h>

namespace eshape
{

class RectShape : public ee::Shape, public s2::RectShape
{
public:
	RectShape();
	RectShape(const RectShape& rect);
	RectShape(const sm::rect& r);
	RectShape(const sm::vec2& center, float hwidth, float hheight);

	/**
	 *  @interface
	 *    s2::Shape
	 */
	virtual RectShape* Clone() const { return new RectShape(*this); }
	virtual bool IsContain(const sm::vec2& pos) const { return s2::RectShape::IsContain(pos); }
	virtual bool IsIntersect(const sm::rect& rect) const { return s2::RectShape::IsIntersect(rect); }
	virtual void Draw(const sm::mat4& mt, const s2::RenderColor* color = NULL) const { s2::RectShape::Draw(mt, color); }

	/**
	 *  @interface
	 *    ee::Shape
	 */
	virtual const char* GetShapeDesc() const { return "rect"; }
	virtual void Translate(const sm::vec2& offset);
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	void SetRect(const sm::rect& r) { s2::RectShape::SetRect(r); }

protected:
	virtual void UpdateBounding() { s2::RectShape::UpdateBounding(); }

}; // RectShape

}

#endif // _EASYSHAPE_RECT_SHAPE_H_