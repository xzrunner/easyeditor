#include "SpriteShader.h"

#include "common/Color.h"

#include <stdio.h>
#include <string.h>
#include <gl/GLee.h>
#include <wx/log.h>

namespace d2d
{

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

static const int ATTRIB_VERTEX    = 0;
static const int ATTRIB_TEXTCOORD = 1;
static const int ATTRIB_COLOR     = 2;
static const int ATTRIB_ADDITIVE  = 3;

int SpriteShader::MAX_COMMBINE = 20000;
//int SpriteShader::MAX_COMMBINE = 4096;

SpriteShader::SpriteShader()
{
	m_model_view = m_projection = 0;
	m_count = 0;
	m_vb = NULL;
	m_color = 0xffffffff;
	m_additive = 0;
	m_tex = m_fbo = 0;
	m_open_buffer_data = true;
	m_is_mat_dirty = false;
}

SpriteShader::~SpriteShader()
{
	delete[] m_vb;
}

void SpriteShader::Load()
{
	static const char* vs =
		FLOAT_PRECISION
		"attribute vec4 position;  \n"
		"attribute vec2 texcoord;  \n"
		"attribute vec4 color;     \n"
		"attribute vec4 additive;  \n"
		"\n"
		"varying vec2 v_texcoord;  \n"
		"varying vec4 v_fragmentColor;  \n"
		"varying vec4 v_fragmentAddi; \n"
		"\n"
		"uniform mat4 u_projection; \n"
		"uniform mat4 u_modelview; \n"
		"\n"
		"void main()  \n"
		"{  \n"
		"  gl_Position = u_projection * u_modelview * position; "
		"  v_fragmentColor = color / 255.0; \n"
		"  v_fragmentAddi = additive / 255.0; \n"
		"  v_texcoord = texcoord;  \n"
		"}  \n"
		;

	static const char* fs =
		FLOAT_PRECISION
		"varying vec4 v_fragmentColor; \n"
		"varying vec4 v_fragmentAddi; \n"
		"varying vec2 v_texcoord;  \n"
		"uniform sampler2D texture0;  \n"
		"\n"
		"void main()  \n"
		"{  \n"  
   		"  vec4 tmp = texture2D(texture0, v_texcoord);  \n"
   		"  gl_FragColor.xyz = tmp.xyz * v_fragmentColor.xyz;  \n"
   		"  gl_FragColor.w = tmp.w;    \n"
   		"  gl_FragColor *= v_fragmentColor.w;  \n"
   		"  gl_FragColor.xyz += v_fragmentAddi.xyz * tmp.w;  \n"
		"}  \n"
		;

	// 	glEnable(GL_BLEND);
	// 	// todo 源混合因子ejoy2d用的GL_ONE
	// 	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	// 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	InitShader(fs, vs);

	m_projection = glGetUniformLocation(m_prog, "u_projection");
	m_model_view = glGetUniformLocation(m_prog, "u_modelview");

	InitBuffers();

	m_vb = new float[SPRITE_FLOAT_NUM * MAX_COMMBINE];
}

void SpriteShader::Unload()
{
	glDeleteBuffers(1, &m_vertex_buffer);
	glDeleteBuffers(1, &m_index_buffer);

	delete[] m_vb;
	m_vb = NULL;
}

void SpriteShader::Bind()
{
	glEnable(GL_BLEND);

	// todo 源混合因子ejoy2d用的GL_ONE
	//		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(m_prog);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);

	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, GL_FALSE, SPRITE_FLOAT_NUM, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(ATTRIB_TEXTCOORD);
	glVertexAttribPointer(ATTRIB_TEXTCOORD, 2, GL_FLOAT, GL_FALSE, SPRITE_FLOAT_NUM, BUFFER_OFFSET(8));

	glEnableVertexAttribArray(ATTRIB_COLOR);
	glVertexAttribPointer(ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_FALSE, SPRITE_FLOAT_NUM, BUFFER_OFFSET(16));

	glEnableVertexAttribArray(ATTRIB_ADDITIVE);
	glVertexAttribPointer(ATTRIB_ADDITIVE, 4, GL_UNSIGNED_BYTE, GL_FALSE, SPRITE_FLOAT_NUM, BUFFER_OFFSET(20));  
}

void SpriteShader::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(ATTRIB_VERTEX);
	glDisableVertexAttribArray(ATTRIB_TEXTCOORD);
	glDisableVertexAttribArray(ATTRIB_COLOR);
	glDisableVertexAttribArray(ATTRIB_ADDITIVE);
}

void SpriteShader::SetModelView(const Vector& offset, float scale)
{
	m_mat_modelview.setScale(scale, scale);
	m_mat_modelview.translate(offset.x, offset.y);
	m_is_mat_dirty = true;
}

void SpriteShader::SetProjection(int width, int height)
{
	float hw = width * 0.5f;
	float hh = height * 0.5f;
	m_mat_projection.orthographic(-hw, hw, -hh, hh, 1, -1);
	m_is_mat_dirty = true;
}

void SpriteShader::Commit()
{
	if (m_count == 0) {
		return;
	}

	if (m_fbo != 0 || (m_fbo != 1 && m_tex != 1)) {
		wxLogDebug(_T("fbo = %d, tex = %d"), m_fbo, m_tex);
	}

	if (m_is_mat_dirty) {
		glUniformMatrix4fv(m_model_view, 1, 0, m_mat_modelview.getElements());
		glUniformMatrix4fv(m_projection, 1, 0, m_mat_projection.getElements());
		m_is_mat_dirty = false;
	}

	static int last_count = 0;
	if (m_open_buffer_data) {
		last_count = m_count;
		glBufferData(GL_ARRAY_BUFFER, m_count * SPRITE_FLOAT_NUM * sizeof(float), &m_vb[0], GL_DYNAMIC_DRAW);
	}

	if (!m_open_buffer_data) {
		glDrawElements(GL_TRIANGLES, 6 * last_count, GL_UNSIGNED_SHORT, 0);
	} else {
		glDrawElements(GL_TRIANGLES, 6 * m_count, GL_UNSIGNED_SHORT, 0);
	}

	m_count = 0;
}

int SpriteShader::GetTexID() const 
{ 
	return m_tex; 
}

void SpriteShader::SetTexID(int tex)
{
	if (m_tex != tex) 
	{
		if (m_count != 0) {
			wxLogDebug(_T("Shader Commit SetTexture %d to %d"), m_tex, tex);
		}

		Commit();
		m_tex = (GLuint)tex;

		glBindTexture(GL_TEXTURE_2D, m_tex);
	}
}

int SpriteShader::GetFboID() const 
{ 
	return m_fbo; 
}

void SpriteShader::SetFboID(int fbo)
{
	if (m_fbo != fbo) 
	{
		if (m_count != 0) {
			wxLogDebug(_T("Shader Commit SetFBO %d to %d"), m_fbo, fbo);
		}

		Commit();
		m_fbo = (GLuint)fbo;
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
	}
}

void SpriteShader::SetBufferData(bool open) 
{
	m_open_buffer_data = open;
}

bool SpriteShader::IsOpenBufferData() const 
{ 
	return m_open_buffer_data; 
}

void SpriteShader::SetColor(const Colorf& multi, const Colorf& add)
{
	m_color = ((int)(multi.a * 255 + 0.5f) << 24) | 
		((int)(multi.b * 255 + 0.5f) << 16) | 
		((int)(multi.g * 255 + 0.5f) << 8) | 
		((int)(multi.r * 255 + 0.5f));
	m_additive = ((int)(add.a * 255 + 0.5f) << 24) | 
		((int)(add.b * 255 + 0.5f) << 16) | 
		((int)(add.g * 255 + 0.5f) << 8) | 
		((int)(add.r * 255 + 0.5f));
}

void SpriteShader::Draw(const float vb[16], int texid)
{
	SetTexID(texid);

	CopyVertex(vb);
	if (++m_count >= MAX_COMMBINE) {
		if (m_count != 0) {
			wxLogDebug(_T("Shader Commit count to max"));
		}
		Commit();
	}
}

void SpriteShader::Draw(const Vector vertices[4], const Vector texcoords[4], int texid)
{
	if (!m_open_buffer_data) {
		return;
	}

	float vb[16];
	for (int j = 0; j < 4; ++j)
	{
		vb[j*4] = vertices[j].x;
		vb[j*4+1] = vertices[j].y;
		vb[j*4+2] = texcoords[j].x;
		vb[j*4+3] = texcoords[j].y;
	}
	Draw(vb, texid);
}

void SpriteShader::BindAttrib(GLuint prog)
{
  	glBindAttribLocation(prog, ATTRIB_VERTEX, "position");
  	glBindAttribLocation(prog, ATTRIB_TEXTCOORD, "texcoord");
  	glBindAttribLocation(prog, ATTRIB_COLOR, "color");
  	glBindAttribLocation(prog, ATTRIB_ADDITIVE, "additive");
}

void SpriteShader::InitBuffers()
{
	glGenBuffers(1, &m_index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);

	size_t size = MAX_COMMBINE * 6 * sizeof(GLushort);
	GLushort* idxs = new GLushort[size];
	for (int i = 0; i < MAX_COMMBINE; ++i) 
	{
		idxs[i*6] = i*4;
		idxs[i*6+1] = i*4+1;
		idxs[i*6+2] = i*4+2;
		idxs[i*6+3] = i*4;
		idxs[i*6+4] = i*4+2;
		idxs[i*6+5] = i*4+3;
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, idxs, GL_STATIC_DRAW);
	delete[] idxs;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_vertex_buffer);
}

void SpriteShader::CopyVertex(const float vb[16])
{
	float* ptr = m_vb + SPRITE_FLOAT_NUM * m_count;
	memcpy(ptr, vb, 4 * sizeof(float));
	ptr += 4;
	memcpy(ptr, &m_color, sizeof(int));
	ptr += 1;
	memcpy(ptr, &m_additive, sizeof(int));
	ptr += 1;  
	memcpy(ptr, &vb[4], 4 * sizeof(float));
	ptr += 4;
	memcpy(ptr, &m_color, sizeof(int));
	ptr += 1;
	memcpy(ptr, &m_additive, sizeof(int));
	ptr += 1;    
	memcpy(ptr, &vb[8], 4 * sizeof(float));
	ptr += 4;
	memcpy(ptr, &m_color, sizeof(int));
	ptr += 1;
	memcpy(ptr, &m_additive, sizeof(int));
	ptr += 1;    
	memcpy(ptr, &vb[12], 4 * sizeof(float));
	ptr += 4;
	memcpy(ptr, &m_color, sizeof(int));
	ptr += 1; 
	memcpy(ptr, &m_additive, sizeof(int));		
}

}