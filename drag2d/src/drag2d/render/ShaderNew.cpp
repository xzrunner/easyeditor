#include "ShaderNew.h"

#include "common/Vector.h"

#include <GL/GLee.h>
#include <stdio.h>
#include <wx/wx.h>

namespace d2d
{

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

static const int ATTRIB_VERTEX    = 0;
static const int ATTRIB_TEXTCOORD = 1;
static const int ATTRIB_COLOR     = 2;
static const int ATTRIB_ADDITIVE  = 3;

ShaderNew* ShaderNew::m_instance = NULL;

int ShaderNew::MAX_COMMBINE = 20000;
//int ShaderNew::MAX_COMMBINE = 4096;

ShaderNew* ShaderNew::Instance()
{
	if (!m_instance)
	{
		m_instance = new ShaderNew();
		m_instance->load();
	}
	return m_instance;
}

ShaderNew::ShaderNew()
{
	m_prog_curr = 0;
	m_prog_sprite = 0;
	m_prog_shape = 0;

	VertexBuffer = IndexBuffer = 0;
	m_sprite_count = 0;
	m_vb = NULL;
	m_tex = 0;
	m_fbo = 0;

	m_color = 0xffffffff;
	m_additive = 0;

	m_version = 0;

	m_open_buffer_data = true;
}

void ShaderNew::SetSpriteColor(const Colorf& multi, const Colorf& add)
{
	// ABGR
	m_color = ((int)(multi.a * 255 + 0.5f) << 24) | 
		((int)(multi.b * 255 + 0.5f) << 16) | 
		((int)(multi.g * 255 + 0.5f) << 8) | 
		((int)(multi.r * 255 + 0.5f));
	m_additive = ((int)(add.a * 255 + 0.5f) << 24) | 
		((int)(add.b * 255 + 0.5f) << 16) | 
		((int)(add.g * 255 + 0.5f) << 8) | 
		((int)(add.r * 255 + 0.5f));
}

void ShaderNew::SetShapeColor(const Colorf& col)
{
	if (m_prog_curr == m_prog_shape) {
		glUniform4fv(m_col_loc, 1, (GLfloat*)(&col.r));
	}
}

void ShaderNew::sprite()
{
	if (m_prog_curr != m_prog_sprite) {
		if (m_sprite_count != 0) {
			wxLogDebug(_T("Shader Commit change shader to sprite"));
		}

		Commit();

		glEnable(GL_BLEND);

		// todo 源混合因子ejoy2d用的GL_ONE
		//		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(m_prog_sprite);
		m_prog_curr = m_prog_sprite;
	}
}

void ShaderNew::shape()
{
 	if (m_prog_curr != m_prog_shape) {
 		if (m_sprite_count != 0) {
 //			wxLogDebug(_T("Shader Commit change shader to shape"));
 		}
 
 		Commit();
 
 		glEnable(GL_BLEND);
 		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 
 		glUseProgram(m_prog_shape);
 		m_prog_curr = m_prog_shape;
 	}
}

void ShaderNew::null()
{
	if (m_sprite_count != 0) {
		wxLogDebug(_T("Shader Commit change shader to null"));
	}

	Commit();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(0);
	m_prog_curr = 0;
}

void ShaderNew::release()
{
	unload();
}

void ShaderNew::reload()
{
	load();
}

void ShaderNew::SetTexture(int tex)
{
	if (m_tex != tex) 
	{
		if (m_sprite_count != 0) {
			wxLogDebug(_T("Shader Commit SetTexture %d to %d"), m_tex, tex);
		}

		Commit();
		m_tex = (GLuint)tex;
		glBindTexture(GL_TEXTURE_2D, m_tex);
	}
}

void ShaderNew::SetFBO(int fbo)
{
	if (m_fbo != fbo) 
	{
		if (m_sprite_count != 0) {
			wxLogDebug(_T("Shader Commit SetFBO %d to %d"), m_fbo, fbo);
		}

		Commit();
		m_fbo = (GLuint)fbo;
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
	}
}

void ShaderNew::Draw(const float vb[16], int texid)
{
	SetTexture(texid);

	CopyVertex(vb);
	if (++m_sprite_count >= MAX_COMMBINE) {
		if (m_sprite_count != 0) {
			wxLogDebug(_T("Shader Commit count to max"));
		}
		Commit();
	}
}

void ShaderNew::Draw(const Vector vertices[4], const Vector texcoords[4], int texid)
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

void ShaderNew::SetModelView(const Vector& offset, float scale)
{
	m_mat_modelview.setScale(scale, scale);
	m_mat_modelview.translate(offset.x, offset.y);
}

void ShaderNew::SetProjection(int width, int height)
{
 	float hw = width * 0.5f;
 	float hh = height * 0.5f;
 	m_mat_projection.orthographic(-hw, hw, -hh, hh, 1, -1);
}

void ShaderNew::Flush()
{
	++m_version;

// 	if (m_sprite_count != 0) {
// 		wxLogDebug(_T("Shader Commit Flush"));
// 	}

	Commit();

//	DynamicTexture::Instance()->DebugDraw();
}

void ShaderNew::load()
{
#if 0
	#define FLOAT_PRECISION \
	"#ifdef GL_FRAGMENT_PRECISION_HIGH  \n" \
	"precision highp float;  \n" \
	"#else  \n" \
	"precision lowp float;  \n" \
	"#endif  \n"
#else
	#define FLOAT_PRECISION \
	"precision lowp float;  \n"
#endif

	static const char * sprite_vs =
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

	static const char * sprite_fs =
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

	static const char* font_fs = 
		FLOAT_PRECISION
		"varying vec4 v_fragmentColor; \n"
		"varying vec2 v_texcoord;  \n"
		"uniform sampler2D texture0;  \n"
		"\n"
		"void main()  \n"
		"{  \n"  
		"  float mask = texture2D(texture0, v_texcoord).a;    \n"
		"  vec4 clr;                                          \n"
		"  clr.rgb = vec3(1.0, 0.0, 0.0);                     \n"
		"  if (mask < 0.5)				                      \n"
		"    clr.a = 0.0;                                     \n"
		"  else                                               \n"
		"    clr.a = 1.0;                                     \n"
		"  gl_FragColor = clr;                                \n"
		"}  \n"
		;

	static const char * shape_vs =
		FLOAT_PRECISION
		"uniform vec4 color;     \n"
		"\n"
		"varying vec4 v_fragmentColor;  \n"
		"\n"
		"void main()  \n"
		"{  \n"
		"  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; "
		"  v_fragmentColor = color; \n"
		"}  \n"
		;

	static const char * shape_fs =
		FLOAT_PRECISION
		"varying vec4 v_fragmentColor; \n"
		"\n"
		"void main()  \n"
		"{  \n"  
		"  gl_FragColor = v_fragmentColor;"
		"}  \n"
		;


	glEnable(GL_BLEND);
	// todo 源混合因子ejoy2d用的GL_ONE
	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_prog_sprite = InitShader(sprite_fs, sprite_vs);
	m_prog_shape = InitShader(shape_fs, shape_vs);
	m_prog_font = InitShader(font_fs, sprite_vs);

 	m_projection = glGetUniformLocation(m_prog_sprite, "u_projection");
 	m_model_view = glGetUniformLocation(m_prog_sprite, "u_modelview");

	InitBuffers();

	// bind attr
	m_col_loc = glGetUniformLocation(m_prog_shape, "color");

	m_vb = new float[SPRITE_FLOAT_NUM * MAX_COMMBINE];
}

void ShaderNew::unload()
{
// 	delete m_instance;
// 	m_instance = NULL;

	// todo others
	glDeleteBuffers(1, &VertexBuffer);
	glDeleteBuffers(1, &IndexBuffer);

	delete[] m_vb;
	m_vb = NULL;
}

int ShaderNew::InitShader(const char *FS, const char *VS)
{
	GLuint prog = glCreateProgram();

	GLuint fs = CompileShader(FS, GL_FRAGMENT_SHADER);
	if (fs == 0) {
		return 0;
	} else {
		glAttachShader(prog, fs);
	}

	GLuint vs = CompileShader(VS, GL_VERTEX_SHADER);
	if (vs == 0) {
		return 0;
	} else {
		glAttachShader(prog, vs);
	}

 	// attr location
 	glBindAttribLocation(prog, ATTRIB_VERTEX, "position");
 	glBindAttribLocation(prog, ATTRIB_TEXTCOORD, "texcoord");
 	glBindAttribLocation(prog, ATTRIB_COLOR, "color");
 	glBindAttribLocation(prog, ATTRIB_ADDITIVE, "additive");

	// link
	GLint status;
	glLinkProgram(prog);

	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	if (status == 0) {
		return 0;
	}

	// detatch
	glDetachShader(prog, fs);
	glDeleteShader(fs);
	glDetachShader(prog, vs);
	glDeleteShader(vs);

	return prog;
}

int ShaderNew::CompileShader(const char * source, int type)
{
	GLint status;

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE) {
		char buf[1024];
		GLint len;
		glGetShaderInfoLog(shader, 1024, &len, buf);

		printf("compile failed:%s\n"
			"source:\n %s\n",
			buf, source);
		glDeleteShader(shader);
		return 0;
	}

	return shader;	
}

void ShaderNew::InitBuffers()
{
	glGenBuffers(1, &IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

	size_t size = MAX_COMMBINE * 6 * sizeof(GLushort);
	GLushort* idxs = new GLushort[size];
	int i;
	for (i=0;i<MAX_COMMBINE;i++) 
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

	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
}

void ShaderNew::Commit()
{
 	if (m_sprite_count == 0) {
 		return;
 	}

	if (m_fbo != 0 || (m_fbo != 1 && m_tex != 1)) {
		wxLogDebug(_T("fbo = %d, tex = %d"), m_fbo, m_tex);
	}

	//////////////////////////////////////////////////////////////////////////
	// Set the model-view transform.
	glUniformMatrix4fv(m_model_view, 1, 0, m_mat_modelview.getElements());

	// Set the projection transform.
	glUniformMatrix4fv(m_projection, 1, 0, m_mat_projection.getElements());
	//////////////////////////////////////////////////////////////////////////

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	static int last_count = 0;
	if (m_open_buffer_data) {
		last_count = m_sprite_count;
 		glBufferData(GL_ARRAY_BUFFER, m_sprite_count * SPRITE_FLOAT_NUM * sizeof(float), &m_vb[0], GL_DYNAMIC_DRAW);
	}

	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, GL_FALSE, SPRITE_FLOAT_NUM, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(ATTRIB_TEXTCOORD);
	glVertexAttribPointer(ATTRIB_TEXTCOORD, 2, GL_FLOAT, GL_FALSE, SPRITE_FLOAT_NUM, BUFFER_OFFSET(8));

	glEnableVertexAttribArray(ATTRIB_COLOR);
	glVertexAttribPointer(ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_FALSE, SPRITE_FLOAT_NUM, BUFFER_OFFSET(16));

	glEnableVertexAttribArray(ATTRIB_ADDITIVE);
	glVertexAttribPointer(ATTRIB_ADDITIVE, 4, GL_UNSIGNED_BYTE, GL_FALSE, SPRITE_FLOAT_NUM, BUFFER_OFFSET(20));  

 	if (!m_open_buffer_data) {
 		glDrawElements(GL_TRIANGLES, 6 * last_count, GL_UNSIGNED_SHORT, 0);
 	} else {
 		glDrawElements(GL_TRIANGLES, 6 * m_sprite_count, GL_UNSIGNED_SHORT, 0);
 	}

 	glBindBuffer(GL_ARRAY_BUFFER, 0);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(ATTRIB_VERTEX);
	glDisableVertexAttribArray(ATTRIB_TEXTCOORD);
	glDisableVertexAttribArray(ATTRIB_COLOR);
	glDisableVertexAttribArray(ATTRIB_ADDITIVE);

	m_sprite_count = 0;
}

void ShaderNew::CopyVertex(const float vb[16])
{
	float* ptr = m_vb + SPRITE_FLOAT_NUM * m_sprite_count;
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