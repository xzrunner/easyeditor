#ifndef _LIBSHAPE_POLYGON_SHAPE_H_
#define _LIBSHAPE_POLYGON_SHAPE_H_

#include "ChainShape.h"

namespace libshape
{

class Material;

class PolygonShape : public ChainShape
{
public:
	PolygonShape();
	PolygonShape(const PolygonShape& polygon);
	PolygonShape(const std::vector<d2d::Vector>& vertices);
	virtual ~PolygonShape();
	
	//
	// IObject interface
	//
	virtual PolygonShape* Clone() const;

	//
	// IShape interface
	//
	virtual const char* GetShapeDesc() const { return "polygon"; }
	virtual bool IsContain(const d2d::Vector& pos) const;
	// todo
	//virtual bool isIntersect(const d2d::Rect& rect) const;
	virtual void Translate(const d2d::Vector& offset);
	virtual void Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color) const;
	virtual d2d::IPropertySetting* CreatePropertySetting(d2d::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;
	virtual void ReloadTexture();

	//
	// ChainShape interface
	//
	virtual void refresh();

	void SetMaterialColor(const d2d::Colorf& color);
	void SetMaterialTexture(d2d::ImageSymbol* image);

	Json::Value StoreMaterial(const std::string& dirpath) const;
	void LoadMaterial(const std::string& dirpath, const Json::Value& val);

	const Material* GetMaterial() const { return m_material; }

protected:
	Material* m_material;

}; // PolygonShape

}

#endif // _LIBSHAPE_POLYGON_SHAPE_H_