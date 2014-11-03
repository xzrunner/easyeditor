#include "DynamicPacker.h"
#include "ShaderMgr.h"

#include "common/Exception.h"
#include "common/Vector.h"
#include "dataset/TPNode.h"
#include "render/PrimitiveDraw.h"
#include "render/style_config.h"

#include <opengl/opengl.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <wx/wx.h>

namespace d2d
{

DynamicPacker::DynamicPacker(int width, int height, int padding)
	: m_width(width)
	, m_height(height)
	, m_padding(padding)
{
	initOpenGLExtensions();

	InitTexture();
	InitFBO();
	InitRoot();
}

DynamicPacker::~DynamicPacker()
{
	glDeleteTextures(1, &m_tex);
	glDeleteFramebuffersEXT(1, &m_fbo);

	delete m_root;
}

void DynamicPacker::ReloadTexture()
{
	wxLogDebug(_T("DynamicPacker ReloadTexture"));

	InitTexture(m_tex);
	InitFBO(m_fbo);
	ReloadPixels();
}

void DynamicPacker::DebugDrawStatic() const
{
	ShaderMgr* shader = ShaderMgr::Instance();
	shader->SetFBO(0);
	shader->sprite();
	shader->SetSpriteColor(Colorf(1, 1, 1, 1), Colorf(0, 0, 0, 0));

	float vb[16];

	vb[0] = 0, vb[1] = 0;
	vb[2] = 0, vb[3] = 0;

	vb[4] = 0, vb[5] = 1;
	vb[6] = 0, vb[7] = 1;

	vb[8] = 1, vb[9] = 1;
	vb[10] = 1, vb[11] = 1;

	vb[12] = 1, vb[13] = 0;
	vb[14] = 1, vb[15] = 0;

	ShaderMgr::Instance()->Draw(vb, m_tex);
}

void DynamicPacker::DebugDraw() const
{
	const int EDGE = 1024;

	Vector vertices[4];
	vertices[0].set(0, 0);
	vertices[1].set(0, EDGE);
	vertices[2].set(EDGE, EDGE);
	vertices[3].set(EDGE, 0);

	ShaderMgr* shader = ShaderMgr::Instance();
	shader->SetFBO(0);
	shader->sprite();
	shader->SetSpriteColor(Colorf(1, 1, 1, 1), Colorf(0, 0, 0, 0));

	float vb[16];

	vb[0] = vertices[0].x, vb[1] = vertices[0].y;
	vb[2] = 0, vb[3] = 0;

	vb[4] = vertices[1].x, vb[5] = vertices[1].y;
	vb[6] = 0, vb[7] = 1;

	vb[8] = vertices[2].x, vb[9] = vertices[2].y;
	vb[10] = 1, vb[11] = 1;

	vb[12] = vertices[3].x, vb[13] = vertices[3].y;
	vb[14] = 1, vb[15] = 0;

	ShaderMgr::Instance()->Draw(vb, m_tex);

	PrimitiveDraw::rect(Vector(0, 0), Vector(EDGE, EDGE), LIGHT_RED_THIN_LINE);
}

void DynamicPacker::InitTexture(int tex_id)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	if (tex_id == 0) {
		glGenTextures(1, (GLuint*)&tex_id);
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_id);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)m_width, (GLsizei)m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
// 	byte* empty_data = new byte[m_width*m_height*4];
// 	memset(empty_data, 0, m_width*m_height*4);
// 	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei)m_width, (GLsizei)m_height, GL_RGBA, GL_UNSIGNED_BYTE, &empty_data[0]);
// 	delete[] empty_data;

	m_tex = tex_id;

//	ClearTexture();
}

void DynamicPacker::InitFBO(int fbo_id)
{
	ShaderMgr* shader = ShaderMgr::Instance();

	if (fbo_id == 0) {
		glGenFramebuffersEXT(1, (GLuint*)&fbo_id);
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_id);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_tex, 0);

	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
		throw Exception("Create FBO error: %d", status);
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ShaderMgr::Instance()->GetFboID());

	m_fbo = fbo_id;
}

void DynamicPacker::InitRoot()
{
	m_root = new TPNode(m_width, m_height);
	TPNode* c = new TPNode(m_width, m_height);
	m_root->SetChild(c);
}

void DynamicPacker::ClearTexture()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
	glBindTexture(GL_TEXTURE_2D, m_tex);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	ShaderMgr* shader = ShaderMgr::Instance();
	glBindTexture(GL_TEXTURE_2D, shader->GetTexID());
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, shader->GetFboID());
}

}