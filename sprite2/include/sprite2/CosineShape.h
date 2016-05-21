#ifndef _SPRITE2_COSINE_SHAPE_H_
#define _SPRITE2_COSINE_SHAPE_H_

#include "PolylineShape.h"

namespace s2
{

class CosineShape : public PolylineShape
{
public:
	CosineShape();
	CosineShape(const std::vector<sm::vec2>& vertices);

	virtual void Draw(const sm::mat4& mt, 
		const RenderColor& color = RenderColor()) const;

private:
	void UpdatePolyline();

private:
	std::vector<sm::vec2> m_mid_points;

}; // CosineShape

}

#endif // _SPRITE2_COSINE_SHAPE_H_