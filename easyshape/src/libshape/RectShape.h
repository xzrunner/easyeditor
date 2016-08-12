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
	RectShape(const sm::rect& r);
	RectShape(const sm::vec2& center, float hwidth, float hheight);

	/**
	 *  @interface
	 *    ee::Shape
	 */
	virtual RectShape* EEClone() const { return new RectShape(*this); }
	virtual const char* GetShapeDesc() const { return "rect"; }
	virtual void Translate(const sm::vec2& offset);
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	void SetRect(const sm::rect& r) { s2::RectShape::SetRect(r); }

}; // RectShape

}

#endif // _EASYSHAPE_RECT_SHAPE_H_