#include "BruteForceTex.h"

#include <SM_Vector.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Model3Shader.h>

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

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::MODEL3);
	sl::Model3Shader* shader = static_cast<sl::Model3Shader*>(mgr->GetShader());

	sl::Model3Shader::Material material;
	material.ambient.Assign(0.04f, 0.04f, 0.04f);
	material.diffuse.Assign(1, 1, 1);
	material.specular.Assign(1, 1, 1);
	material.shininess = 50;
	material.tex_id = m_texture.GetID();
	shader->SetMaterial(material);

	float height;
	for (int y = 0; y < m_data.size - 1; ++y)
	{
		int size = (m_data.size - 1) * 2;

		std::vector<float> vertices(5 * size);
		size_t v_ptr = 0;

		std::vector<uint16_t> indices((size - 1) * 3);
		size_t i_ptr = 0;

		for (int x = 0; x < m_data.size - 1; ++x)
		{
 			//calculate the texture coordinates
 			float tex_left   = (float)x/m_data.size;
 			float tex_bottom = (float)y/m_data.size;
 			float tex_top    = (float)(y+1)/m_data.size;

			height = GetScaledHeightAtPoint(x, y);
			vertices[v_ptr++] = scale*x;
			vertices[v_ptr++] = scale*y;
			vertices[v_ptr++] = height;
			vertices[v_ptr++] = tex_left;
			vertices[v_ptr++] = tex_bottom;

			height = GetScaledHeightAtPoint(x, y+1);
			vertices[v_ptr++] = scale*x;
			vertices[v_ptr++] = scale*(y+1);
			vertices[v_ptr++] = height;
			vertices[v_ptr++] = tex_left;
			vertices[v_ptr++] = tex_top;

			if (x == 0) {
				indices[i_ptr++] = 0;
				indices[i_ptr++] = 1;
			} else {
				uint16_t curr = i_ptr;
				indices[i_ptr++] = curr;
				indices[i_ptr++] = indices[curr];
				indices[i_ptr++] = indices[curr - 1];
				indices[i_ptr++] = indices[curr + 1];
			}
		}

		shader->Draw(&vertices[0], vertices.size(), &indices[0], indices.size(), false, true);		
	}
}

}