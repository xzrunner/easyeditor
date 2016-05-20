#ifndef _EASYSHAPE_RECT_SHAPE_H_
#define _EASYSHAPE_RECT_SHAPE_H_

#include <ee/Shape.h>

#include <sprite2/RectShape.h>

namespace eshape
{

class RectShape : public ee::Shape
{
public:
	RectShape();
	RectShape(const sm::rect& r);
	RectShape(const sm::vec2& center, float hWidth, float hHeight);

	//
	// IObject interface
	//
	virtual RectShape* Clone() const;

	//
	// Shape interface
	//
	virtual const char* GetShapeDesc() const { return "rect"; }
	virtual bool IsContain(const sm::vec2& pos) const { return m_core.IsContain(pos); }
	virtual bool IsIntersect(const sm::rect& rect) const { return m_core.IsIntersect(rect); }
	virtual void Translate(const sm::vec2& offset);
	virtual const sm::rect& GetRect() const { return m_core.GetRegion(); }
	virtual void Draw(const sm::mat4& mt, const s2::RenderColor& color = s2::RenderColor()) const {
		m_core.Draw(mt, color);
	}
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	void SetRect(const sm::rect& r) { m_core.SetRect(r); }

private:
	s2::RectShape m_core;

}; // RectShape

}

#endif // _EASYSHAPE_RECT_SHAPE_H_