#include "BruteForce.h"

#include <SM_Vector.h>
#include <sprite2/Color.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Shape3Shader.h>

namespace eterrain3d
{

void BruteForce::Draw() const
{
	if (!m_data.pixels) {
		return;
	}

	const float scale = 0.1f;

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SHAPE3);
	auto shader = static_cast<sl::Shape3Shader*>(mgr->GetShader());

	float col, height;
	for (int y = 0; y < m_data.size - 1; ++y)
	{
		int size = (m_data.size - 1) * 2;
		std::vector<s2::Color> colors;
		colors.reserve(size);
		std::vector<sm::vec3> vertices;
		vertices.reserve(size);
		for (int x = 0; x < m_data.size - 1; ++x)
		{
			col = GetTrueHeightAtPoint(x, y);
			colors.push_back(s2::Color(col, col, col));
			height = GetScaledHeightAtPoint(x, y);
			vertices.push_back(sm::vec3(scale*x, scale*y, height));

			col = GetTrueHeightAtPoint(x, y+1);
			colors.push_back(s2::Color(col, col, col));
			height = GetScaledHeightAtPoint(x, y+1);
			vertices.push_back(sm::vec3(scale*x, scale*(y+1), height));
		}

		// todo
//		mgr->DrawShape(GL_TRIANGLE_STRIP, &vertices[0].x, size, &colors[0], true);	
	}
}

}