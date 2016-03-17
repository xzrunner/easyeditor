#include "BruteForceTex.h"

namespace eterrain3d
{

void BruteForceTex::Draw() const
{
	if (!m_data.pixels) {
		return;
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture.GetID());

	const float scale = 0.1f;

	e3d::ShaderMgr* mgr = e3d::ShaderMgr::Instance();
	mgr->Sprite();

	float height;
	for (int y = 0; y < m_data.size - 1; ++y)
	{
		int size = (m_data.size - 1) * 2;
		std::vector<ee::vec3> vertices;
		vertices.reserve(size);
		std::vector<ee::vec2> texcoords;
		texcoords.reserve(size);
		for (int x = 0; x < m_data.size - 1; ++x)
		{
 			//calculate the texture coordinates
 			float tex_left   = (float)x/m_data.size;
 			float tex_bottom = (float)y/m_data.size;
 			float tex_top    = (float)(y+1)/m_data.size;

			height = GetScaledHeightAtPoint(x, y);
			vertices.push_back(ee::vec3(scale*x, scale*y, height));
			texcoords.push_back(ee::vec2(tex_left, tex_bottom));

			height = GetScaledHeightAtPoint(x, y+1);
			vertices.push_back(ee::vec3(scale*x, scale*(y+1), height));
			texcoords.push_back(ee::vec2(tex_left, tex_top));
		}

		mgr->DrawTriStrip(&vertices[0].x, &texcoords[0].x, size, m_texture.GetID());
	}
}

}