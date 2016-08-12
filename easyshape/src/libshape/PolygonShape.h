#ifndef _EASYSHAPE_POLYGON_SHAPE_H_
#define _EASYSHAPE_POLYGON_SHAPE_H_

#include "PolylineShape.h"

#include <sprite2/PolylineShape.h>

namespace ee { class ImageSymbol; }

namespace eshape
{

class Material;

class PolygonShape : public PolylineShape, public s2::PolylineShape
{
public:
	PolygonShape();
	PolygonShape(const std::vector<sm::vec2>& vertices);
	PolygonShape(const PolygonShape& polygon);
	PolygonShape& operator = (const PolygonShape& polygon);
	virtual ~PolygonShape();
	
	/**
	 *  @interface
	 *    s2::Shape
	 */
	virtual void Draw(const sm::mat4& mt, 
		const s2::RenderColor& color = s2::RenderColor()) const;

	/**
	 *  @interface
	 *    ee::Shape
	 */
	virtual PolygonShape* Clone() const { return new PolygonShape(*this); }
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

	const Material* GetMaterial() const { return m_material; }

//	const std::vector<sm::vec2>& GetVertices() const { return m_vertices; }

protected:
	Material* m_material;

}; // PolygonShape

}

#endif // _EASYSHAPE_POLYGON_SHAPE_H_