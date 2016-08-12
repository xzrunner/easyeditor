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
	CosineShape(const std::vector<sm::vec2>& vertices);

	/**
	 *  @interface
	 *    ee::Shape
	 */
	virtual CosineShape* Clone() const { return new CosineShape(*this); }
	virtual const char* GetShapeDesc() const { return "cosine-curve"; }
	virtual void Translate(const sm::vec2& offset);
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage) { return NULL; }
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir) {}
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const {}

//	const std::vector<sm::vec2>& getMidPoints() const { return m_midPoints; }

}; // CosineShape

}

#endif // _EASYSHAPE_COSINE_SHAPE_H_