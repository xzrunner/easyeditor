#include <gl/glew.h>

#include "SpriteShader.h"
#include "Camera.h"

#include <wx/log.h>

namespace e3d
{

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

static const int ATTRIB_VERTEX    = 0;
static const int ATTRIB_TEXTCOORD = 1;

static const int VERTEX_FLOAT_SIZE = 3 + 2;
static const int VERTEX_SIZE = VERTEX_FLOAT_SIZE * sizeof(float);
static const int MAX_VERTEX = 10240;

SpriteShader::SpriteShader()
{
	m_model_view = m_projection = 0;
	m_vertex_buffer = 0;
	m_count = 0;
	m_vb = 0;
	m_tex = 0;
}

SpriteShader::~SpriteShader()
{
	delete m_vb;
}

void SpriteShader::Load()
{
	static const char* vs =
		FLOAT_PRECISION
		"attribute ee::vec4 position;  \n"
		"attribute ee::vec2 texcoord;  \n"
		"\n"
		"varying ee::vec2 v_texcoord;  \n"
		"\n"
		"\n"
		"uniform ee::mat4 u_projection; \n"
		"uniform ee::mat4 u_modelview; \n"
		"\n"
		"void main()  \n"
		"{  \n"
		"  gl_Position = u_projection * u_modelview * gl_Vertex; "
		"  v_texcoord = texcoord;  \n"
		"}  \n"
		;

	static const char* fs =
		FLOAT_PRECISION
		"varying ee::vec2 v_texcoord;  \n"
		"uniform sampler2D texture0;  \n"
		"\n"
		"void main()  \n"
		"{  \n"  
		"  gl_FragColor = texture2D(texture0, v_texcoord); \n"
		"}  \n"
		;

	InitShader(vs, fs);

	m_projection = glGetUniformLocation(m_prog, "u_projection");
	m_model_view = glGetUniformLocation(m_prog, "u_modelview");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glGenBuffers(1, &m_vertex_buffer);

	m_vb = new float[VERTEX_FLOAT_SIZE * MAX_VERTEX];
}

void SpriteShader::Unload()
{
	glDeleteBuffers(1, &m_vertex_buffer);

	delete[] m_vb;
	m_vb = NULL;
}

void SpriteShader::Bind()
{
	glUseProgram(m_prog);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

 	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
 
 	glEnableVertexAttribArray(ATTRIB_VERTEX);
 	glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, 20, BUFFER_OFFSET(0));
 
 	glEnableVertexAttribArray(ATTRIB_TEXTCOORD);
 	glVertexAttribPointer(ATTRIB_TEXTCOORD, 2, GL_FLOAT, GL_FALSE, 20, BUFFER_OFFSET(12));
}

void SpriteShader::Unbind()
{
	glDisable(GL_DEPTH_TEST);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(ATTRIB_VERTEX);
	glDisableVertexAttribArray(ATTRIB_TEXTCOORD);
}

void SpriteShader::SetModelView(const ee::mat4& mat)
{
	m_mat_modelview = mat;
}

void SpriteShader::SetProjection(int width, int height)
{
	float hh = 1.0f * height / width;
	m_mat_projection = ee::mat4::Perspective(-1, 1, -hh, hh, 
		Camera::CAM_NEAR, Camera::CAM_FAR);
}

void SpriteShader::Commit()
{
	if (m_count == 0) {
		return;
	}

	glUniformMatrix4fv(m_model_view, 1, 0, m_mat_modelview.Pointer());
	glUniformMatrix4fv(m_projection, 1, 0, m_mat_projection.Pointer());

	// todo test dirty
	glBufferData(GL_ARRAY_BUFFER, m_count * VERTEX_SIZE, &m_vb[0], GL_DYNAMIC_DRAW);

	glDrawArrays(m_type, 0, m_count);

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

void SpriteShader::DrawTri(const ee::vec3 vertices[3], const ee::vec2 texcoords[3], int texid)
{
	SetTexID(texid);

	if (m_count + 3 >= MAX_VERTEX) {
		wxLogDebug(_T("SpriteShader Commit count to max"));
		Commit();
	} else if (m_type != GL_TRIANGLES) {
		wxLogDebug(_T("SpriteShader Commit change type"));
		Commit();
		m_type = GL_TRIANGLES;
	}

	CopyVertex(vertices, texcoords);
}

void SpriteShader::DrawTri(const float* vertices, const float* texcoords, int count, int texid)
{
	if (count >= MAX_VERTEX) {
		return;
	}

	SetTexID(texid);

	if (m_count + count >= MAX_VERTEX) {
		wxLogDebug(_T("SpriteShader Commit count to max"));
		Commit();
	} else if (m_type != GL_TRIANGLES) {
		wxLogDebug(_T("SpriteShader Commit change type"));
		Commit();
		m_type = GL_TRIANGLES;
	}

	CopyVertex(vertices, texcoords, count);

	// force flush
	Commit();
}

void SpriteShader::DrawTriStrip(float* vertices, float* texcoords, int count, int texid)
{
	if (count >= MAX_VERTEX) {
		return;
	}

	SetTexID(texid);

	if (m_count + count >= MAX_VERTEX) {
		wxLogDebug(_T("SpriteShader Commit count to max"));
		Commit();
	} else if (m_type != GL_TRIANGLE_STRIP) {
		wxLogDebug(_T("SpriteShader Commit change type"));
		Commit();
		m_type = GL_TRIANGLE_STRIP;
	}

	CopyVertex(vertices, texcoords, count);

	// force flush
	Commit();
}

void SpriteShader::BindAttrib(GLuint prog)
{
	glBindAttribLocation(prog, ATTRIB_VERTEX, "position");
	glBindAttribLocation(prog, ATTRIB_TEXTCOORD, "texcoord");
}

void SpriteShader::CopyVertex(const ee::vec3 vertices[3], const ee::vec2 texcoords[3])
{
	float* ptr = m_vb + VERTEX_FLOAT_SIZE * m_count;
	memcpy(ptr, &vertices[0].x, sizeof(ee::vec3));
	ptr += 3;
	memcpy(ptr, &texcoords[0].x, sizeof(ee::vec2));
	ptr += 2;
	memcpy(ptr, &vertices[1].x, sizeof(ee::vec3));
	ptr += 3;
	memcpy(ptr, &texcoords[1].x, sizeof(ee::vec2));
	ptr += 2;
	memcpy(ptr, &vertices[2].x, sizeof(ee::vec3));
	ptr += 3;
	memcpy(ptr, &texcoords[2].x, sizeof(ee::vec2));
	ptr += 2;
	m_count += 3;
}

void SpriteShader::CopyVertex(const float* vertices, const float* texcoords, int count)
{
	float* ptr = m_vb + VERTEX_FLOAT_SIZE * m_count;
	for (int i = 0; i < count; ++i)
	{
		memcpy(ptr, &vertices[i*3], sizeof(ee::vec3));
		ptr += 3;
		memcpy(ptr, &texcoords[i*2], sizeof(ee::vec2));
		ptr += 2;
		++m_count;
	}
}

}