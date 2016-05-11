#ifndef _EASYSHAPE_COSINE_SHAPE_H_
#define _EASYSHAPE_COSINE_SHAPE_H_

#include "ChainShape.h"

namespace eshape
{

class CosineShape : public ChainShape
{
public:
	CosineShape();
	CosineShape(const CosineShape& curve);
	CosineShape(const std::vector<sm::vec2>& vertices);
	virtual ~CosineShape();

	//
	// IObject interface
	//
	virtual CosineShape* Clone() const;

	//
	// Shape interface
	//
	virtual const char* GetShapeDesc() const { return "cosine-curve"; }
	virtual void Translate(const sm::vec2& offset);
	virtual void Draw(const sm::mat4& mt,
		const s2::RenderColor& color = s2::RenderColor()) const;
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);

	//
	// ChainShape interface
	//
	virtual void insertVertices(size_t index, const sm::vec2& pos);
	virtual void removeVertices(const sm::vec2& pos);
	virtual void changeVertices(const sm::vec2& from, const sm::vec2& to);
	virtual void refresh();

	const std::vector<sm::vec2>& getMidPoints() const { return m_midPoints; }
	void setMidPoints();

private:
	static const float SAMPLING_WIDTH;

private:
	std::vector<sm::vec2> m_midPoints;

}; // CosineShape

}

#endif // _EASYSHAPE_COSINE_SHAPE_H_