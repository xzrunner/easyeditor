#ifndef _EASYSHAPE_RECT_SHAPE_H_
#define _EASYSHAPE_RECT_SHAPE_H_

#include <ee/Shape.h>

namespace eshape
{

class RectShape : public ee::Shape
{
public:
	RectShape();
	RectShape(const RectShape& rect);
	RectShape(const sm::vec2& p0, const sm::vec2& p1);
	RectShape(const sm::vec2& center, float hWidth, float hHeight);

	//
	// IObject interface
	//
	virtual RectShape* Clone() const;

	//
	// Shape interface
	//
	virtual const char* GetShapeDesc() const { return "rect"; }
	virtual bool IsContain(const sm::vec2& pos) const;
	virtual bool IsIntersect(const sm::rect& rect) const;
	virtual void Translate(const sm::vec2& offset);
	virtual const sm::rect& GetRect() const { return m_rect; }
	virtual void Draw(const sm::mat4& mt, const s2::RenderColor& color = s2::RenderColor()) const;
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

public:
	sm::rect m_rect;

}; // RectShape

}

#endif // _EASYSHAPE_RECT_SHAPE_H_