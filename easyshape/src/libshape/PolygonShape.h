#ifndef _LIBSHAPE_POLYGON_SHAPE_H_
#define _LIBSHAPE_POLYGON_SHAPE_H_

#include "ChainShape.h"

namespace d2d { class ImageSymbol; }

namespace libshape
{

class PolygonShape : public ChainShape
{
public:
	enum FillingType
	{
		e_Color = 0,
		e_Texture,
	};

public:
	PolygonShape(const PolygonShape& polygon);
	PolygonShape(const std::vector<d2d::Vector>& vertices);
	virtual ~PolygonShape();
	
	//
	// IObject interface
	//
	virtual PolygonShape* clone() const;

	//
	// IShape interface
	//
	virtual void draw(const d2d::Screen& scr, const d2d::Colorf& color = d2d::Colorf(0, 0, 0)) const;
	virtual d2d::IPropertySetting* createPropertySetting(d2d::EditPanel* editPanel);

	//
	// ChainShape interface
	//
	virtual void refresh();

public:
	void buildFillingTris();
	void buildFillingTris(const std::vector<d2d::Vector>& segments);

private:
	void getBoundingRegion(const std::vector<d2d::Vector>& bounding, float& left, float& right, float& low, float& up);
	void getTextureBoundarySegments(float left, float right, float low, float up,
		std::vector<d2d::Vector>& segments);
	void computeTextureCoords(float left, float low);

public:
	int m_fillingType;

	d2d::Colorf m_fillingColor;
	d2d::ImageSymbol* m_fillingTexture;

	std::vector<d2d::Vector> m_fillingVertices;
	std::vector<d2d::Vector> m_fillingTexCoords;

}; // PolygonShape

}

#endif // _LIBSHAPE_POLYGON_SHAPE_H_