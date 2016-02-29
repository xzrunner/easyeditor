#include "SpriteShader.h"
#include "ColorTrans.h"
#include "Color.h"

#define STRINGIFY(A)  #A
#include "SpriteShader.vert"
#include "SpriteShader.frag"
#include "SpriteFasterShader.frag"

#include <sl_sprite.h>

#include <gl/glew.h>

#include <string>

#include <stdio.h>

namespace ee
{

static const int ATTRIB_VERTEX    = 0;
static const int ATTRIB_TEXTCOORD = 1;
static const int ATTRIB_COLOR     = 2;
static const int ATTRIB_ADDITIVE  = 3;
static const int ATTRIB_R_TRANS   = 4;
static const int ATTRIB_G_TRANS   = 5;
static const int ATTRIB_B_TRANS   = 6;

SpriteShader::SpriteShader()
{
	m_vertex_size = (4 + 2 + 3) * sizeof(float);
	m_quad_size = (4 + 2 + 3) * 4;

	m_max_commbine = 20000;	// 4096

	m_model_view = m_projection = 0;
	m_vertex_buffer = m_index_buffer = 0;
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
	//////////////////////////////////////////////////////////////////////////
 	sl_sprite_load();
// 	sl_sprite_modelview(0, 0, 1, 1);
	//////////////////////////////////////////////////////////////////////////

// 	LoadShader();
// 
// 	m_projection = glGetUniformLocation(m_prog, "u_projection");
// 	m_model_view = glGetUniformLocation(m_prog, "u_modelview");
// 
// 	InitBuffers();
// 
// 	if (!m_vb) {
// 		m_vb = new float[m_quad_size * m_max_commbine];
// 	}
}

void SpriteShader::Unload()
{
	//////////////////////////////////////////////////////////////////////////
	sl_sprite_unload();
	//////////////////////////////////////////////////////////////////////////

// 	glDeleteBuffers(1, &m_vertex_buffer);
// 	glDeleteBuffers(1, &m_index_buffer);
// 
// 	delete[] m_vb;
// 	m_vb = NULL;
}

void SpriteShader::Bind()
{
	//////////////////////////////////////////////////////////////////////////
	sl_sprite_bind();
	//////////////////////////////////////////////////////////////////////////

// 	glUseProgram(m_prog);
// 
// 	glEnable(GL_BLEND);
// 	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
// 
// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
// 	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
// 
// 	glEnableVertexAttribArray(ATTRIB_VERTEX);
// 	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, GL_FALSE, m_vertex_size, BUFFER_OFFSET(0));
// 
// 	glEnableVertexAttribArray(ATTRIB_TEXTCOORD);
// 	glVertexAttribPointer(ATTRIB_TEXTCOORD, 2, GL_FLOAT, GL_FALSE, m_vertex_size, BUFFER_OFFSET(8));
// 
// 	glEnableVertexAttribArray(ATTRIB_COLOR);
// 	glVertexAttribPointer(ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_FALSE, m_vertex_size, BUFFER_OFFSET(16));
// 
// 	glEnableVertexAttribArray(ATTRIB_ADDITIVE);
// 	glVertexAttribPointer(ATTRIB_ADDITIVE, 4, GL_UNSIGNED_BYTE, GL_FALSE, m_vertex_size, BUFFER_OFFSET(20));  
// 
// 	glEnableVertexAttribArray(ATTRIB_R_TRANS);
// 	glVertexAttribPointer(ATTRIB_R_TRANS, 4, GL_UNSIGNED_BYTE, GL_FALSE, m_vertex_size, BUFFER_OFFSET(24));  
// 
// 	glEnableVertexAttribArray(ATTRIB_G_TRANS);
// 	glVertexAttribPointer(ATTRIB_G_TRANS, 4, GL_UNSIGNED_BYTE, GL_FALSE, m_vertex_size, BUFFER_OFFSET(28));  
// 
// 	glEnableVertexAttribArray(ATTRIB_B_TRANS);
// 	glVertexAttribPointer(ATTRIB_B_TRANS, 4, GL_UNSIGNED_BYTE, GL_FALSE, m_vertex_size, BUFFER_OFFSET(32));  
}

void SpriteShader::Unbind()
{
	//////////////////////////////////////////////////////////////////////////
	sl_sprite_unbind();
	//////////////////////////////////////////////////////////////////////////

//// 	SetTexID(0);
//// 	SetFboID(0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glDisableVertexAttribArray(ATTRIB_VERTEX);
//	glDisableVertexAttribArray(ATTRIB_TEXTCOORD);
//	glDisableVertexAttribArray(ATTRIB_COLOR);
//	glDisableVertexAttribArray(ATTRIB_ADDITIVE);
//	glDisableVertexAttribArray(ATTRIB_R_TRANS);
//	glDisableVertexAttribArray(ATTRIB_G_TRANS);
//	glDisableVertexAttribArray(ATTRIB_B_TRANS);
}

void SpriteShader::SetModelView(const Vector& offset, float scale)
{
	//////////////////////////////////////////////////////////////////////////
	sl_sprite_modelview(offset.x, offset.y, scale, scale);
	//////////////////////////////////////////////////////////////////////////

// 	m_mat_modelview.SetScale(scale, scale);
// 	m_mat_modelview.Translate(offset.x, offset.y);
// 	m_is_mat_dirty = true;
}

void SpriteShader::SetProjection(int width, int height)
{
	//////////////////////////////////////////////////////////////////////////
	sl_sprite_projection(width, height);
	//////////////////////////////////////////////////////////////////////////

// 	float hw = width * 0.5f;
// 	float hh = height * 0.5f;
// 	m_mat_projection.Orthographic(-hw, hw, -hh, hh, 1, -1);
// 	m_is_mat_dirty = true;
}

void SpriteShader::Commit()
{
	//////////////////////////////////////////////////////////////////////////
	sl_sprite_commit();
	//////////////////////////////////////////////////////////////////////////

// 	if (m_count == 0) {
// 		return;
// 	}
// 
// 	if (m_is_mat_dirty) {
// 		glUniformMatrix4fv(m_model_view, 1, 0, m_mat_modelview.GetElements());
// 		glUniformMatrix4fv(m_projection, 1, 0, m_mat_projection.GetElements());
// 		m_is_mat_dirty = false;
// 	}
// 
// 	static int last_count = 0;
// 	if (m_open_buffer_data) {
// 		last_count = m_count;
// 		glBufferData(GL_ARRAY_BUFFER, m_count * m_quad_size * sizeof(float), &m_vb[0], GL_DYNAMIC_DRAW);
// 	}
// 
// 	if (!m_open_buffer_data) {
// 		glDrawElements(GL_TRIANGLES, 6 * last_count, GL_UNSIGNED_SHORT, 0);
// 	} else {
// 		glDrawElements(GL_TRIANGLES, 6 * m_count, GL_UNSIGNED_SHORT, 0);
// 	}
// 
// 	m_count = 0;
}

void SpriteShader::Reset()
{
	m_tex = 0;
	m_fbo = 0;
}

int SpriteShader::GetTexID() const 
{ 
	return m_tex; 
}

void SpriteShader::SetTexID(int tex)
{
	if (m_tex != tex) 
	{
		Commit();
		m_tex = (GLuint)tex;

		glActiveTexture(GL_TEXTURE0);
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

void SpriteShader::SetColor(const ColorTrans& color)
{
	m_color = 
		((int)(color.multi.a * 255 + 0.5f) << 24) | 
		((int)(color.multi.b * 255 + 0.5f) << 16) | 
		((int)(color.multi.g * 255 + 0.5f) << 8) | 
		((int)(color.multi.r * 255 + 0.5f));

	m_additive = 
		((int)(color.add.a * 255 + 0.5f) << 24) | 
		((int)(color.add.b * 255 + 0.5f) << 16) | 
		((int)(color.add.g * 255 + 0.5f) << 8) | 
		((int)(color.add.r * 255 + 0.5f));

	m_r_trans = 
		((int)(color.r.a * 255 + 0.5f) << 24) | 
		((int)(color.r.b * 255 + 0.5f) << 16) | 
		((int)(color.r.g * 255 + 0.5f) << 8) | 
		((int)(color.r.r * 255 + 0.5f));

	m_g_trans = 
		((int)(color.g.a * 255 + 0.5f) << 24) | 
		((int)(color.g.b * 255 + 0.5f) << 16) | 
		((int)(color.g.g * 255 + 0.5f) << 8) | 
		((int)(color.g.r * 255 + 0.5f));

	m_b_trans = 
		((int)(color.b.a * 255 + 0.5f) << 24) | 
		((int)(color.b.b * 255 + 0.5f) << 16) | 
		((int)(color.b.g * 255 + 0.5f) << 8) | 
		((int)(color.b.r * 255 + 0.5f));
}

void SpriteShader::Draw(const float vb[16], int texid)
{
// 	SetTexID(texid);
// 
// 	CopyVertex(vb);
// 	if (++m_count >= m_max_commbine) {
// 		Commit();
// 	}
}

void SpriteShader::Draw(const Vector vertices[4], const Vector texcoords[4], int texid)
{
	//////////////////////////////////////////////////////////////////////////
	sl_sprite_draw(&vertices[0].x, &texcoords[0].x, texid);
	//////////////////////////////////////////////////////////////////////////

// 	if (!m_open_buffer_data) {
// 		return;
// 	}
// 
// 	float vb[16];
// 	for (int j = 0; j < 4; ++j)
// 	{
// 		vb[j*4] = vertices[j].x;
// 		vb[j*4+1] = vertices[j].y;
// 		vb[j*4+2] = texcoords[j].x;
// 		vb[j*4+3] = texcoords[j].y;
// 	}
// 	Draw(vb, texid);
}

void SpriteShader::BindAttribLocation(GLuint prog)
{
//   	glBindAttribLocation(prog, ATTRIB_VERTEX, "position");
//   	glBindAttribLocation(prog, ATTRIB_TEXTCOORD, "texcoord");
//   	glBindAttribLocation(prog, ATTRIB_COLOR, "color");
//   	glBindAttribLocation(prog, ATTRIB_ADDITIVE, "additive");
// 	glBindAttribLocation(prog, ATTRIB_R_TRANS, "r_trans");
// 	glBindAttribLocation(prog, ATTRIB_G_TRANS, "g_trans");
// 	glBindAttribLocation(prog, ATTRIB_B_TRANS, "b_trans");
}

void SpriteShader::LoadShader()
{
// 	static const std::string header(FLOAT_PRECISION);
// 	static const std::string vert(header + std::string(SpriteVertShader));
// 	static const std::string frag(header + SpriteFragShader);
// 
// 	InitShader(vert.c_str(), frag.c_str());
}

void SpriteShader::InitBuffers()
{
// 	glGenBuffers(1, &m_index_buffer);
// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
// 
// 	size_t size = m_max_commbine * 6 * sizeof(GLushort);
// 	GLushort* idxs = new GLushort[size];
// 	for (int i = 0; i < m_max_commbine; ++i) 
// 	{
// 		idxs[i*6] = i*4;
// 		idxs[i*6+1] = i*4+1;
// 		idxs[i*6+2] = i*4+2;
// 		idxs[i*6+3] = i*4;
// 		idxs[i*6+4] = i*4+2;
// 		idxs[i*6+5] = i*4+3;
// 	}
// 
// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, idxs, GL_STATIC_DRAW);
// 	delete[] idxs;
// 
// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
// 
// 	glGenBuffers(1, &m_vertex_buffer);
}

void SpriteShader::CopyVertex(const float vb[16])
{
// 	float* ptr = m_vb + m_quad_size * m_count;
// 	for (int i = 0; i < 4; ++i)
// 	{
// 		memcpy(ptr, &vb[i*4], 4 * sizeof(float));
// 		ptr += 4;
// 		memcpy(ptr, &m_color, sizeof(int));
// 		ptr += 1;
// 		memcpy(ptr, &m_additive, sizeof(int));
// 		ptr += 1;
// 		memcpy(ptr, &m_r_trans, sizeof(int));
// 		ptr += 1;
// 		memcpy(ptr, &m_g_trans, sizeof(int));
// 		ptr += 1;
// 		memcpy(ptr, &m_b_trans, sizeof(int));
// 		ptr += 1;
// 	}	
}

}