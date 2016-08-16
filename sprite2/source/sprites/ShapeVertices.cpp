#include "ShapeVertices.h"
#include "RenderColor.h"
#include "S2_RVG.h"

namespace s2
{

void ShapeVertices::Draw(const std::vector<sm::vec2>& vertices, bool closed,
						 const sm::mat4& mt, const RenderColor& color)
{
	if (vertices.empty()) {
		return;
	}

	std::vector<sm::vec2> trans(vertices);
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		trans[i] = mt * vertices[i];
	}
	RVG::SetColor(color.mul);
	RVG::Polyline(trans, closed);
}

}