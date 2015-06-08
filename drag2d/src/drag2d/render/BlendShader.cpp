#include "BlendShader.h"

#define STRINGIFY(A)  #A
#include "BlendShader.vert"
#include "BlendShader.frag"

#include <gl/glew.h>
#include <string>
#include <wx/log.h>

namespace d2d
{

static const int ATTRIB_TEXTCOORD_BASE   = 7;

BlendShader::BlendShader()
	: m_mode(BM_UNKNOWN)
	, m_mode_id(0)
{
	m_vertex_size += 4 * 2;
}

void BlendShader::Bind()
{
	SpriteShader::Bind();

 	glEnableVertexAttribArray(ATTRIB_TEXTCOORD_BASE);
 	glVertexAttribPointer(ATTRIB_TEXTCOORD_BASE, 2, GL_FLOAT, GL_FALSE, m_vertex_size, BUFFER_OFFSET(36));
}

void BlendShader::Unbind()
{
	SpriteShader::Unbind();

	glDisableVertexAttribArray(ATTRIB_TEXTCOORD_BASE);
}

void BlendShader::SetBaseTexID(int tex)
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex);
	glActiveTexture(GL_TEXTURE0);
}

void BlendShader::SetBlendMode(const std::string& str)
{
	BlendMode mode = BlendModes::Instance()->GetIDFromNameEN(str);
	if (mode == BM_UNKNOWN) {
		mode = BM_NORMAL;
	}

	if (mode != m_mode)
	{
		Commit();
		m_mode = mode;
		glUniform1i(m_mode_id, GLint(m_mode));

		glUniform1i(m_sampler0, 0);
		glUniform1i(m_sampler1, 1);
	}
}

void BlendShader::DrawBlend(const float vb[24], int texid)
{
	SetTexID(texid);

	CopyVertex(vb);
	if (++m_count >= m_max_commbine) {
		if (m_count != 0) {
			wxLogDebug(_T("Shader Commit count to max"));
		}
		Commit();
	}
}

void BlendShader::DrawBlend(const Vector vertices[4], const Vector texcoords[4], 
							const Vector texcoords_base[4], int texid)
{
	float vb[24];
	for (int j = 0; j < 4; ++j)
	{
		vb[j*6] = vertices[j].x;
		vb[j*6+1] = vertices[j].y;
		vb[j*6+2] = texcoords[j].x;
		vb[j*6+3] = texcoords[j].y;
		vb[j*6+4] = texcoords_base[j].x;
		vb[j*6+5] = texcoords_base[j].y;
	}
	DrawBlend(vb, texid);
}

void BlendShader::GetUniformLocation(GLuint prog)
{
	m_mode_id = glGetUniformLocation(m_prog, "u_mode");
	m_sampler0 = glGetUniformLocation(m_prog, "texture0");
	m_sampler1 = glGetUniformLocation(m_prog, "texture1");
}

void BlendShader::LoadShader()
{
	static const std::string header(FLOAT_PRECISION);
	static const std::string vert(header + BlendVertShader);
	static const std::string frag(header + BlendFragShader);

	InitShader(vert.c_str(), frag.c_str());
}

void BlendShader::CopyVertex(const float vb[24])
{
	float* ptr = m_vb + m_vertex_size * m_count;
	for (int i = 0; i < 4; ++i)
	{
		memcpy(ptr, &vb[i*6], 4 * sizeof(float));
		ptr += 4;
		memcpy(ptr, &m_color, sizeof(int));
		ptr += 1;
		memcpy(ptr, &m_additive, sizeof(int));
		ptr += 1;
		memcpy(ptr, &m_r_trans, sizeof(int));
		ptr += 1;
		memcpy(ptr, &m_g_trans, sizeof(int));
		ptr += 1;
		memcpy(ptr, &m_b_trans, sizeof(int));
		ptr += 1;
		memcpy(ptr, &vb[i*6+4], 2 * sizeof(float));
		ptr += 2;
	}	
}

}