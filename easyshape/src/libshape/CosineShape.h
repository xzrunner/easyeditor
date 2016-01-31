#ifndef _LIBSHAPE_COSINE_SHAPE_H_
#define _LIBSHAPE_COSINE_SHAPE_H_

#include "ChainShape.h"

namespace libshape
{

class CosineShape : public ChainShape
{
public:
	CosineShape();
	CosineShape(const CosineShape& curve);
	CosineShape(const std::vector<ee::Vector>& vertices);
	virtual ~CosineShape();

	//
	// IObject interface
	//
	virtual CosineShape* Clone() const;

	//
	// Shape interface
	//
	virtual const char* GetShapeDesc() const { return "cosine-curve"; }
	virtual void Translate(const ee::Vector& offset);
	virtual void Draw(const ee::Matrix& mt,
		const ee::ColorTrans& color = ee::ColorTrans()) const;
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);

	//
	// ChainShape interface
	//
	virtual void insertVertices(size_t index, const ee::Vector& pos);
	virtual void removeVertices(const ee::Vector& pos);
	virtual void changeVertices(const ee::Vector& from, const ee::Vector& to);
	virtual void refresh();

	const std::vector<ee::Vector>& getMidPoints() const { return m_midPoints; }
	void setMidPoints();

private:
	static const float SAMPLING_WIDTH;

private:
	std::vector<ee::Vector> m_midPoints;

}; // CosineShape

}

#endif // _LIBSHAPE_COSINE_SHAPE_H_