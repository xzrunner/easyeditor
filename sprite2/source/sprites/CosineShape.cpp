#include "CosineShape.h"
#include "ShapeVertices.h"

#include <SM_CosineSmooth.h>
#include <SM_DouglasPeucker.h>

namespace s2
{

static const float SAMPLING_WIDTH = 10.0f;

CosineShape::CosineShape()
{
}

CosineShape::CosineShape(const std::vector<sm::vec2>& vertices)
	: PolylineShape(vertices)
{
	UpdatePolyline();
}

CosineShape* CosineShape::Clone() const
{
	return new CosineShape(*this);
}

void CosineShape::Draw(const sm::mat4& mt, const RenderColor& color) const
{
	ShapeVertices::Draw(m_mid_points, m_closed, mt, color);
}

void CosineShape::UpdatePolyline()
{
	if (m_vertices.size() <= 1) {
		return;
	}

	m_mid_points.clear();

	std::vector<sm::vec2> smooth;
	sm::cosine_smooth(m_vertices, SAMPLING_WIDTH, smooth);

	sm::douglas_peucker(smooth, 0.75f, m_mid_points);
}

}