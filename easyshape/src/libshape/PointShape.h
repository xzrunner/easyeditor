#ifndef _EASYSHAPE_POINT_SHAPE_H_
#define _EASYSHAPE_POINT_SHAPE_H_

#include <ee/Shape.h>

#include <cu/cu_stl.h>
#include <sprite2/PointShape.h>

namespace eshape
{

class PointShape : public ee::Shape, public s2::PointShape
{
public:
	PointShape() {}
	PointShape(const PointShape& point);
	PointShape(const sm::vec2& pos);

	/**
	 *  @interface
	 *    s2::Shape
	 */
	virtual PointShape* Clone() const override { return new PointShape(*this); }
	virtual bool IsContain(const sm::vec2& pos) const override { return s2::PointShape::IsContain(pos); }
	virtual bool IsIntersect(const sm::rect& rect) const override { return s2::PointShape::IsIntersect(rect); }
	virtual void Draw(cooking::DisplayList* dlist, const s2::RenderParams& rp) const override { s2::PointShape::Draw(dlist, rp); }

	/**
	 *  @interface
	 *    ee::Shape
	 */
	virtual const char* GetShapeDesc() const override { return "point"; }
	virtual void Translate(const sm::vec2& offset) override { SetPos(m_pos + offset); }
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage) override { return NULL; }
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir) override;
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const override;

protected:
	virtual void UpdateBounding() override { s2::PointShape::UpdateBounding(); }

}; // PointShape

}

#endif // _EASYSHAPE_POINT_SHAPE_H_