#ifndef _EASYSHAPE_RECT_SHAPE_H_
#define _EASYSHAPE_RECT_SHAPE_H_

#include <ee/Shape.h>

#include <sprite2/RectShape.h>

namespace eshape
{

class RectShape : public ee::Shape, private s2::RectShape
{
public:
	RectShape();
	RectShape(const sm::rect& r);
	RectShape(const sm::vec2& center, float hwidth, float hheight);

	//
	// IObject interface
	//
	virtual RectShape* Clone() const;

	//
	// Shape interface
	//
	virtual const char* GetShapeDesc() const { return "rect"; }
	virtual bool IsContain(const sm::vec2& pos) const { return s2::RectShape::IsContain(pos); }
	virtual bool IsIntersect(const sm::rect& rect) const { return s2::RectShape::IsIntersect(rect); }
	virtual void Translate(const sm::vec2& offset);
	virtual const sm::rect& GetRect() const { return s2::RectShape::GetBounding(); }
	virtual void Draw(const sm::mat4& mt, const s2::RenderColor& color = s2::RenderColor()) const {
		s2::RectShape::Draw(mt, color);
	}
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	void SetRect(const sm::rect& r) { s2::RectShape::SetRect(r); }

}; // RectShape

}

#endif // _EASYSHAPE_RECT_SHAPE_H_