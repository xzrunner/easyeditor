#ifndef _EASYSHAPE_POLYGON_SHAPE_H_
#define _EASYSHAPE_POLYGON_SHAPE_H_

#include "ChainShape.h"

namespace ee { class ImageSymbol; }

namespace eshape
{

class Material;

class PolygonShape : public ChainShape
{
public:
	PolygonShape();
	PolygonShape(const PolygonShape& polygon);
	PolygonShape(const std::vector<ee::Vector>& vertices);
	virtual ~PolygonShape();
	
	//
	// IObject interface
	//
	virtual PolygonShape* Clone() const;

	//
	// Shape interface
	//
	virtual const char* GetShapeDesc() const { return "polygon"; }
	virtual bool IsContain(const ee::Vector& pos) const;
	// todo
	//virtual bool isIntersect(const ee::Rect& rect) const;
	virtual void Translate(const ee::Vector& offset);
	virtual void Draw(const ee::Matrix& mt, const ee::ColorTrans& color) const;
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;
	virtual void ReloadTexture();

	//
	// ChainShape interface
	//
	virtual void refresh();

	void SetMaterialColor(const ee::Colorf& color);
	void SetMaterialTexture(ee::ImageSymbol* image);

	Json::Value StoreMaterial(const std::string& dirpath) const;
	void LoadMaterial(const std::string& dirpath, const Json::Value& val);

	const Material* GetMaterial() const { return m_material; }

protected:
	Material* m_material;

}; // PolygonShape

}

#endif // _EASYSHAPE_POLYGON_SHAPE_H_