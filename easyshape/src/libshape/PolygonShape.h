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
	PolygonShape(const std::vector<sm::vec2>& vertices);
	virtual ~PolygonShape();
	
	/**
	 *  @interface
	 *    s2::Shape
	 */
	virtual PolygonShape* Clone() const { return new PolygonShape(*this); }
	virtual bool IsContain(const sm::vec2& pos) const { return s2::PolylineShape::IsContain(pos); }
	virtual bool IsIntersect(const sm::rect& rect) const { return s2::PolylineShape::IsIntersect(rect); }
	virtual void Draw(const sm::mat4& mt, const s2::RenderColor* color = NULL) const;

	/**
	 *  @interface
	 *    ee::Shape
	 */
	virtual const char* GetShapeDesc() const { return "polygon"; }
	virtual void Translate(const sm::vec2& offset);
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	virtual void ReloadTexture();

	/*
	 *  interface PolylineShape
	 */
	virtual void AddVertex(int index, const sm::vec2& pos);
	virtual void RemoveVertex(const sm::vec2& pos);
	virtual void ChangeVertex(const sm::vec2& from, const sm::vec2& to);
	virtual void SetVertices(const std::vector<sm::vec2>& vertices);
	virtual const std::vector<sm::vec2>& GetVertices() const { return m_vertices; }
	virtual bool IsClosed() const { return true; }

	void SetMaterialColor(const s2::Color& color);
	void SetMaterialTexture(ee::ImageSymbol* image);

	Json::Value StoreMaterial(const std::string& dirpath) const;
	void LoadMaterial(const std::string& dirpath, const Json::Value& val);

	const Material* GetMaterial() const;

}; // PolygonShape

}

#endif // _EASYSHAPE_POLYGON_SHAPE_H_