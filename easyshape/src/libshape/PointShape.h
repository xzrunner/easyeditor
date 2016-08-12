#ifndef _EASYSHAPE_POINT_SHAPE_H_
#define _EASYSHAPE_POINT_SHAPE_H_

#include <ee/Shape.h>

#include <sprite2/PointShape.h>

namespace eshape
{

class PointShape : public ee::Shape, public s2::PointShape, public virtual s2::Shape
{
public:
	PointShape() {}
	PointShape(const sm::vec2& pos) 
		: s2::PointShape(pos) 
	{}

	/**
	 *  @interface
	 *    ee::Shape
	 */
	virtual PointShape* EEClone() const { return new PointShape(*this); }
	virtual const char* GetShapeDesc() const { return "point"; }
	virtual void Translate(const sm::vec2& offset) { SetPos(m_pos + offset); }
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage) { return NULL; }
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	const sm::vec2& GetPos() const { 
		return m_pos; 
	}
	void SetPos(const sm::vec2& pos) { 
		m_pos = pos;
		UpdateBounding();
	}

}; // PointShape

}

#endif // _EASYSHAPE_POINT_SHAPE_H_