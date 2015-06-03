#include "SpriteShader.h"

#include "common/Color.h"

#include <stdio.h>
#include <string.h>
#include <gl/glew.h>
#include <wx/log.h>

namespace d2d
{

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

static const int ATTRIB_VERTEX    = 0;
static const int ATTRIB_TEXTCOORD = 1;
static const int ATTRIB_COLOR     = 2;
static const int ATTRIB_ADDITIVE  = 3;
static const int ATTRIB_R_TRANS   = 4;
static const int ATTRIB_G_TRANS   = 5;
static const int ATTRIB_B_TRANS   = 6;

static const int VERTEX_SIZE = 4 * 4 + 4 * 2 + 4 * 3;

static const int MAX_COMMBINE = 20000;
//static const int MAX_COMMBINE = 4096;

SpriteShader::SpriteShader()
{
	m_model_view = m_projection = 0;
	m_count = 0;
	m_vb = NULL;
	m_color = 0xffffffff;
	m_additive = 0;
	m_r_trans = 0xff0000ff;
	m_g_trans = 0xff00ff00;
	m_b_trans = 0xffff0000;
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
	LoadShader();

	m_projection = glGetUniformLocation(m_prog, "u_projection");
	m_model_view = glGetUniformLocation(m_prog, "u_modelview");

	InitBuffers();

	m_vb = new float[VERTEX_SIZE * MAX_COMMBINE];
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
	glUseProgram(m_prog);

	glEnable(GL_BLEND);

	// todo 源混合因子ejoy2d用的GL_ONE
	//		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);

	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(ATTRIB_TEXTCOORD);
	glVertexAttribPointer(ATTRIB_TEXTCOORD, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(8));

	glEnableVertexAttribArray(ATTRIB_COLOR);
	glVertexAttribPointer(ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(16));

	glEnableVertexAttribArray(ATTRIB_ADDITIVE);
	glVertexAttribPointer(ATTRIB_ADDITIVE, 4, GL_UNSIGNED_BYTE, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(20));  

	glEnableVertexAttribArray(ATTRIB_R_TRANS);
	glVertexAttribPointer(ATTRIB_R_TRANS, 4, GL_UNSIGNED_BYTE, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(24));  

	glEnableVertexAttribArray(ATTRIB_G_TRANS);
	glVertexAttribPointer(ATTRIB_G_TRANS, 4, GL_UNSIGNED_BYTE, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(28));  

	glEnableVertexAttribArray(ATTRIB_B_TRANS);
	glVertexAttribPointer(ATTRIB_B_TRANS, 4, GL_UNSIGNED_BYTE, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(32));  
}

void SpriteShader::Unbind()
{
	SetTexID(0);
	SetFboID(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(ATTRIB_VERTEX);
	glDisableVertexAttribArray(ATTRIB_TEXTCOORD);
	glDisableVertexAttribArray(ATTRIB_COLOR);
	glDisableVertexAttribArray(ATTRIB_ADDITIVE);
	glDisableVertexAttribArray(ATTRIB_R_TRANS);
	glDisableVertexAttribArray(ATTRIB_G_TRANS);
	glDisableVertexAttribArray(ATTRIB_B_TRANS);
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

// 	if (m_fbo != 0 || (m_fbo != 1 && m_tex != 1)) {
// 		wxLogDebug(_T("fbo = %d, tex = %d"), m_fbo, m_tex);
// 	}

	if (m_is_mat_dirty) {
		glUniformMatrix4fv(m_model_view, 1, 0, m_mat_modelview.getElements());
		glUniformMatrix4fv(m_projection, 1, 0, m_mat_projection.getElements());
		m_is_mat_dirty = false;
	}

	static int last_count = 0;
	if (m_open_buffer_data) {
		last_count = m_count;
		glBufferData(GL_ARRAY_BUFFER, m_count * VERTEX_SIZE * sizeof(float), &m_vb[0], GL_DYNAMIC_DRAW);
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
			//wxLogDebug(_T("Shader Commit SetTexture %d to %d"), m_tex, tex);
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
	m_color = 
		((int)(multi.a * 255 + 0.5f) << 24) | 
		((int)(multi.b * 255 + 0.5f) << 16) | 
		((int)(multi.g * 255 + 0.5f) << 8) | 
		((int)(multi.r * 255 + 0.5f));

	m_additive = 
		((int)(add.a * 255 + 0.5f) << 24) | 
		((int)(add.b * 255 + 0.5f) << 16) | 
		((int)(add.g * 255 + 0.5f) << 8) | 
		((int)(add.r * 255 + 0.5f));
}

void SpriteShader::SetColorTrans(const Colorf& r_trans, const Colorf& g_trans, const Colorf& b_trans)
{
	m_r_trans = 
		((int)(r_trans.a * 255 + 0.5f) << 24) | 
		((int)(r_trans.b * 255 + 0.5f) << 16) | 
		((int)(r_trans.g * 255 + 0.5f) << 8) | 
		((int)(r_trans.r * 255 + 0.5f));

	m_g_trans = 
		((int)(g_trans.a * 255 + 0.5f) << 24) | 
		((int)(g_trans.b * 255 + 0.5f) << 16) | 
		((int)(g_trans.g * 255 + 0.5f) << 8) | 
		((int)(g_trans.r * 255 + 0.5f));

	m_b_trans = 
		((int)(b_trans.a * 255 + 0.5f) << 24) | 
		((int)(b_trans.b * 255 + 0.5f) << 16) | 
		((int)(b_trans.g * 255 + 0.5f) << 8) | 
		((int)(b_trans.r * 255 + 0.5f));
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
	glBindAttribLocation(prog, ATTRIB_R_TRANS, "r_trans");
	glBindAttribLocation(prog, ATTRIB_G_TRANS, "g_trans");
	glBindAttribLocation(prog, ATTRIB_B_TRANS, "b_trans");
}

void SpriteShader::LoadShader()
{
	static const char* vs =
		FLOAT_PRECISION
		"attribute vec4 position;  \n"
		"attribute vec2 texcoord;  \n"
		"attribute vec4 color;     \n"
		"attribute vec4 additive;  \n"
		"attribute vec4 r_trans;  \n"
		"attribute vec4 g_trans;  \n"
		"attribute vec4 b_trans;  \n"
		"\n"
		"varying vec2 v_texcoord;  \n"
		"varying vec4 v_fragmentColor;  \n"
		"varying vec4 v_fragmentAddi; \n"
		"varying vec4 v_r_trans; \n"
		"varying vec4 v_g_trans; \n"
		"varying vec4 v_b_trans; \n"
		"\n"
		"uniform mat4 u_projection; \n"
		"uniform mat4 u_modelview; \n"
		"\n"
		"void main()  \n"
		"{  \n"
		"  gl_Position = u_projection * u_modelview * position; "
		"  v_fragmentColor = color / 255.0; \n"
		"  v_fragmentAddi = additive / 255.0; \n"
		"  v_r_trans = r_trans / 255.0; \n"
		"  v_g_trans = g_trans / 255.0; \n"
		"  v_b_trans = b_trans / 255.0; \n"
		"  v_texcoord = texcoord;  \n"
		"}  \n"
		;

	static const char* fs =
		FLOAT_PRECISION
		"varying vec4 v_fragmentColor; \n"
		"varying vec4 v_fragmentAddi; \n"
		"varying vec4 v_r_trans; \n"
		"varying vec4 v_g_trans; \n"
		"varying vec4 v_b_trans; \n"
		"varying vec2 v_texcoord;  \n"
		"uniform sampler2D texture0;  \n"
		"\n"
		"void main()  \n"
		"{  \n"  
		"  vec4 tmp = texture2D(texture0, v_texcoord);  \n"

		"  float s = 1.5; \n"
		"  vec4 r = vec4(tmp.r, 0, 0, 1); \n"
		"  vec4 g = vec4(0, tmp.g, 0, 1); \n"
		"  vec4 b = vec4(0, 0, tmp.b, 1); \n"
		"  if ((v_r_trans.r != 1 || v_r_trans.g != 0 || v_r_trans.b != 0) && tmp.r > tmp.g * s && tmp.r > tmp.b * s) { \n"
		"  	  r = tmp.r * v_r_trans; \n"
		"  	  g = tmp.g * v_r_trans; \n"
		"  	  b = tmp.b * v_r_trans; \n"
		"  } \n"
		"  if ((v_g_trans.g != 1 || v_g_trans.r != 0 || v_g_trans.b != 0) && tmp.g > tmp.r * s && tmp.g > tmp.b * s) { \n"
		"  	  g = tmp.g * v_g_trans; \n"
		"  	  r = tmp.r * v_g_trans; \n"
		"  	  b = tmp.b * v_g_trans; \n"
		"  } \n"
		"  if ((v_b_trans.b != 1 || v_b_trans.r != 0 || v_b_trans.g != 0) && tmp.b > tmp.r * s && tmp.b > tmp.g * s) { \n"
		"    b = tmp.b * v_b_trans; \n"
		"    r = tmp.r * v_b_trans; \n"
		"    g = tmp.g * v_b_trans; \n"
		"  } \n"
		"  tmp.xyz = (r + g + b).xyz; \n"

		"  gl_FragColor.xyz = tmp.xyz * v_fragmentColor.xyz;  \n"
		"  gl_FragColor.w = tmp.w;    \n"
		"  gl_FragColor *= v_fragmentColor.w;  \n"
		"  gl_FragColor.xyz += v_fragmentAddi.xyz * tmp.w;  \n"
		"}  \n"
		;

// 	static const char* fs =
// 		FLOAT_PRECISION
// 		"varying vec4 v_fragmentColor; \n"
// 		"varying vec4 v_fragmentAddi; \n"
// 		"varying vec4 v_r_trans; \n"
// 		"varying vec4 v_g_trans; \n"
// 		"varying vec4 v_b_trans; \n"
// 		"varying vec2 v_texcoord;  \n"
// 		"uniform sampler2D texture0;  \n"
// 		"\n"
// 		"void main()  \n"
// 		"{  \n"  
// 		"  vec4 tmp = texture2D(texture0, v_texcoord);  \n"
// 
// 		"  float s = 1.5; \n"
// 
// 		"  vec3 test_v_g_trans = vec3(2, 0.3, 1);  \n"
// 		//"  vec3 test_v_g_trans = vec3(1, 1, 1);  \n"
// 		"  float cmp_r = tmp.g - tmp.r * s;  \n"
// 		"  float test_r = clamp(cmp_r, 0, 1) / cmp_r;  \n"
// 		"  float cmp_b = tmp.g - tmp.b * s;  \n"
// 		"  float test_b = clamp(cmp_b, 0, 1) / cmp_b;  \n"
// 		"  vec3 trans = (test_v_g_trans - vec3(1, 1, 1)) * test_r * test_b + vec3(1, 1, 1);  \n"
// 		"  tmp.xyz = vec3(tmp.r * trans.r, tmp.g * trans.g, tmp.b * trans.b);  \n"
// 
// 		"  gl_FragColor.xyz = tmp.xyz * v_fragmentColor.xyz;  \n"
// 		"  gl_FragColor.w = tmp.w;    \n"
// 		"  gl_FragColor *= v_fragmentColor.w;  \n"
// 		"  gl_FragColor.xyz += v_fragmentAddi.xyz * tmp.w;  \n"
// 		"}  \n"
// 		;

// 		static const char* fs =
// 			FLOAT_PRECISION
// 			"varying vec4 v_fragmentColor; \n"
// 			"varying vec4 v_fragmentAddi; \n"
// 			"varying vec4 v_r_trans; \n"
// 			"varying vec4 v_g_trans; \n"
// 			"varying vec4 v_b_trans; \n"
// 			"varying vec2 v_texcoord;  \n"
// 			"uniform sampler2D texture0;  \n"
// 			"\n"
// 			"void main()  \n"
// 			"{  \n"  
// 			"  vec4 tmp = texture2D(texture0, v_texcoord);  \n"
// 
// 			"  float s = 1.5; \n"
// 			"  vec3 test_v_g_trans = vec3(2, 0.3, 1);  \n"
// 
// 			"  float cmp_r = step(tmp.r * s, tmp.g);  \n"
// 			"  float cmp_b = step(tmp.b * s, tmp.g);  \n"
// 			"  vec3 trans = (test_v_g_trans - vec3(1, 1, 1)) * cmp_r * cmp_b + vec3(1, 1, 1);  \n"
// 			"  tmp.xyz = vec3(tmp.r * trans.r, tmp.g * trans.g, tmp.b * trans.b);  \n"
// 
// 			"  gl_FragColor.xyz = tmp.xyz * v_fragmentColor.xyz;  \n"
// 			"  gl_FragColor.w = tmp.w;    \n"
// 			"  gl_FragColor *= v_fragmentColor.w;  \n"
// 			"  gl_FragColor.xyz += v_fragmentAddi.xyz * tmp.w;  \n"
// 			"}  \n"
// 			;

	// 	glEnable(GL_BLEND);
	// 	// todo 源混合因子ejoy2d用的GL_ONE
	// 	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	// 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	InitShader(vs, fs);
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
	float* ptr = m_vb + VERTEX_SIZE * m_count;
	for (int i = 0; i < 4; ++i)
	{
		memcpy(ptr, &vb[i*4], 4 * sizeof(float));
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
	}	
}

}