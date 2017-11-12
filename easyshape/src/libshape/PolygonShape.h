#ifndef _EASYSHAPE_POLYGON_SHAPE_H_
#define _EASYSHAPE_POLYGON_SHAPE_H_

#include "EditedPolyShape.h"

#include <sprite2/PolygonShape.h>

namespace ee { class ImageSymbol; }

namespace eshape
{

class Material;

class PolygonShape : public EditedPolyShape, public s2::PolygonShape
{
public:
	PolygonShape();
	PolygonShape(const PolygonShape& polygon);
	PolygonShape(const CU_VEC<sm::vec2>& vertices);
	virtual ~PolygonShape();
	
	/**
	 *  @interface
	 *    s2::Shape
	 */
	virtual PolygonShape* Clone() const override { return new PolygonShape(*this); }
	virtual bool IsContain(const sm::vec2& pos) const override { return s2::PolylineShape::IsContain(pos); }
	virtual bool IsIntersect(const sm::rect& rect) const override { return s2::PolylineShape::IsIntersect(rect); }
	virtual void Draw(const s2::RenderParams& rp) const override;

	/**
	 *  @interface
	 *    ee::Shape
	 */
	virtual const char* GetShapeDesc() const override { return "polygon"; }
	virtual void Translate(const sm::vec2& offset) override;
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage) override;
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir) override;
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const override;

	virtual void ReloadTexture() override;

	/*
	 *  interface PolylineShape
	 */
	virtual void AddVertex(int index, const sm::vec2& pos) override;
	virtual void RemoveVertex(const sm::vec2& pos) override;
	virtual void ChangeVertex(const sm::vec2& from, const sm::vec2& to) override;
	virtual void SetVertices(const CU_VEC<sm::vec2>& vertices) override;
	virtual const CU_VEC<sm::vec2>& GetVertices() const override { return m_vertices; }
	virtual bool IsClosed() const override { return true; }

	void SetMaterialColor(const s2::Color& color);
	void SetMaterialTexture(const std::shared_ptr<ee::ImageSymbol>& image);

	Json::Value StoreMaterial(const std::string& dirpath) const;
	void LoadMaterial(const std::string& dirpath, const Json::Value& val);

	const Material* GetMaterial() const;

}; // PolygonShape

}

#endif // _EASYSHAPE_POLYGON_SHAPE_H_