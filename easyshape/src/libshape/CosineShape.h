#ifndef _LIBSHAPE_COSINE_SHAPE_H_
#define _LIBSHAPE_COSINE_SHAPE_H_

#include "ChainShape.h"

namespace libshape
{

class CosineShape : public ChainShape
{
public:
	CosineShape(const CosineShape& curve);
	CosineShape(const std::vector<d2d::Vector>& vertices);
	virtual ~CosineShape();

	//
	// IObject interface
	//
	virtual CosineShape* clone() const;

	//
	// IShape interface
	//
	virtual void draw(const d2d::Screen& scr, const d2d::Colorf& color = d2d::Colorf(0, 0, 0)) const;
	virtual d2d::IPropertySetting* createPropertySetting(d2d::EditPanel* editPanel);

	//
	// ChainShape interface
	//
	virtual void insertVertices(size_t index, const d2d::Vector& pos);
	virtual void removeVertices(const d2d::Vector& pos);
	virtual void changeVertices(const d2d::Vector& from, const d2d::Vector& to);
	virtual void refresh();

	const std::vector<d2d::Vector>& getMidPoints() const { return m_midPoints; }
	void setMidPoints();

private:
	static const float SAMPLING_WIDTH;

private:
	std::vector<d2d::Vector> m_midPoints;

}; // CosineShape

}

#endif // _LIBSHAPE_COSINE_SHAPE_H_