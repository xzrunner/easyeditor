#include <gl/glew.h>

#include "ShapeShader.h"
#include "Camera.h"

#include <wx/log.h>

namespace e3d
{

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

static const int ATTRIB_VERTEX = 0;
static const int ATTRIB_COLOR  = 1;

static const int VERTEX_FLOAT_SIZE = 3 + 1;
static const int VERTEX_SIZE = VERTEX_FLOAT_SIZE * sizeof(float);
static const int MAX_VERTEX = 10240;

ShapeShader::ShapeShader()
{
	m_model_view = m_projection = 0;
	m_vertex_buffer = 0;
	m_type = 0;
	m_count = 0;
	m_vb = NULL;
}

ShapeShader::~ShapeShader()
{
	delete m_vb;
}

void ShapeShader::Load()
{
	static const char* vs =
		FLOAT_PRECISION
		"attribute sm::vec4 position;  \n"
		"attribute sm::vec4 color;     \n"
		"\n"
		"varying sm::vec4 v_fragmentColor;  \n"
		"\n"
		"\n"
		"uniform sm::mat4 u_projection; \n"
		"uniform sm::mat4 u_modelview; \n"
		"\n"
		"void main()  \n"
		"{  \n"
		"  gl_Position = u_projection * u_modelview * gl_Vertex; "
		"  v_fragmentColor = color / 255.0; \n"
		"}  \n"
		;

	static const char* fs =
		FLOAT_PRECISION
		"varying sm::vec4 v_fragmentColor; \n"
		"\n"
		"void main()  \n"
		"{  \n"  
		"  gl_FragColor = v_fragmentColor;"
		"}  \n"
		;

	InitShader(vs, fs);

	m_projection = glGetUniformLocation(m_prog, "u_projection");
	m_model_view = glGetUniformLocation(m_prog, "u_modelview");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glGenBuffers(1, &m_vertex_buffer);

	m_vb = new float[VERTEX_FLOAT_SIZE * MAX_VERTEX];
}

void ShapeShader::Unload()
{
	glDeleteBuffers(1, &m_vertex_buffer);

	delete[] m_vb;
	m_vb = NULL;
}

void ShapeShader::Bind()
{
	glUseProgram(m_prog);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);

	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(ATTRIB_COLOR);
	glVertexAttribPointer(ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(12));

// 	glEnable(GL_LINE_SMOOTH);
// 	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);                 // Set Line Antialiasing
}

void ShapeShader::Unbind()
{
	glDisable(GL_DEPTH_TEST);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(ATTRIB_VERTEX);
	glDisableVertexAttribArray(ATTRIB_COLOR);
}

void ShapeShader::SetProjection(int width, int height)
{
	float hh = 1.0f * height / width;
	m_mat_projection = sm::mat4::Perspective(-1, 1, -hh, hh, 
		Camera::CAM_NEAR, Camera::CAM_FAR);
}

void ShapeShader::Commit()
{
	if (m_count == 0) {
		return;
	}

	glUniformMatrix4fv(m_model_view, 1, 0, m_mat_modelview.Pointer());
	glUniformMatrix4fv(m_projection, 1, 0, m_mat_projection.Pointer());

	glBufferData(GL_ARRAY_BUFFER, m_count * VERTEX_SIZE, &m_vb[0], GL_DYNAMIC_DRAW);

	glDrawArrays(m_type, 0, m_count);

	m_count = 0;
}

void ShapeShader::SetModelView(const sm::mat4& mat)
{
	m_mat_modelview = mat;
}

void ShapeShader::Draw(int type, const float* vertices, int count, 
					   const s2::Color& col, bool force)
{
	if (m_count + count >= MAX_VERTEX) {
		wxLogDebug(_T("ShapeShader Commit count to max"));
		Commit();
	} else if (type != m_type) {
		wxLogDebug(_T("ShapeShader Commit for type change"));
		Commit();
	} else if (force) {
		Commit();
	}

	m_type = type;
	CopyVertex(vertices, count, PackColor(col));
}

void ShapeShader::Draw(int type, const float* vertices, int count, 
					   s2::Color* cols, bool force)
{
	if (m_count + count >= MAX_VERTEX) {
		wxLogDebug(_T("ShapeShader Commit count to max"));
		Commit();
	} else if (type != m_type) {
		wxLogDebug(_T("ShapeShader Commit for type change"));
		Commit();
	} else if (force) {
		Commit();
	}

	m_type = type;
	CopyVertex(vertices, count, cols);
}

void ShapeShader::Draw(int type, const float* vertices, int vcount, 
					   const s2::Color& col, unsigned short* indices, int icount)
{
	if (vcount >= MAX_VERTEX) {
		return;
	}

	if (m_count + vcount >= MAX_VERTEX) {
		wxLogDebug(_T("ShapeShader Commit count to max"));
		Commit();
	} else {
		Commit();
	}

	CopyVertex(vertices, vcount, PackColor(col));
	Commit(type, indices, icount);
}

void ShapeShader::BindAttrib(GLuint prog)
{
	glBindAttribLocation(prog, ATTRIB_VERTEX, "position");
	glBindAttribLocation(prog, ATTRIB_COLOR, "color");
}

void ShapeShader::CopyVertex(const float* vertices, int count, int color)
{
	float* ptr_dst = m_vb + VERTEX_FLOAT_SIZE * m_count;
	const float* ptr_src = &vertices[0];
	for (int i = 0; i < count; ++i)
	{
		memcpy(ptr_dst, ptr_src, sizeof(float)*3);
		ptr_dst += 3;
		ptr_src += 3;
		memcpy(ptr_dst, &color, sizeof(int));
		ptr_dst += 1;
	}
	m_count += count;
}

void ShapeShader::CopyVertex(const float* vertices, int count, s2::Color* cols)
{
 	float* ptr_dst = m_vb + VERTEX_FLOAT_SIZE * m_count;
 	const float* ptr_src = &vertices[0];
 	for (int i = 0; i < count; ++i)
 	{
 		memcpy(ptr_dst, ptr_src, sizeof(float)*3);
 		ptr_dst += 3;
 		ptr_src += 3;
 
 		int color = PackColor(cols[i]);
 		memcpy(ptr_dst, &color, sizeof(int));
 		ptr_dst += 1;
 	}
 	m_count += count;
}

void ShapeShader::Commit(int type, unsigned short* indices, int count)
{
	if (m_count == 0) {
		return;
	}

	glUniformMatrix4fv(m_model_view, 1, 0, m_mat_modelview.Pointer());
	glUniformMatrix4fv(m_projection, 1, 0, m_mat_projection.Pointer());

	glBufferData(GL_ARRAY_BUFFER, m_count * VERTEX_SIZE, &m_vb[0], GL_DYNAMIC_DRAW);

	glDrawElements(type, count, GL_UNSIGNED_SHORT, indices);

	m_count = 0;
}

int ShapeShader::PackColor(const s2::Color& col)
{
	int color = ((int)(col.a * 255 + 0.5f) << 24) | 
				((int)(col.b * 255 + 0.5f) << 16) | 
				((int)(col.g * 255 + 0.5f) << 8)  | 
				((int)(col.r * 255 + 0.5f));
	return color;
}

}