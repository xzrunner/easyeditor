#include "DynamicTexture.h"

#include "common/tools.h"
#include "common/Exception.h"
#include "dataset/TPNode.h"
#include "render/ShaderNew.h"

//#include <gl/glew.h>

#include <opengl/opengl.h>
#include <gl/gl.h>
#include <gl/glu.h>

namespace d2d
{

const int DynamicTexture::WIDTH = 512;
const int DynamicTexture::HEIGHT = 512;
const int DynamicTexture::PADDING = 1;

DynamicTexture* DynamicTexture::m_instance = NULL;
DynamicTexture* DynamicTexture::Instance()
{
	if (!m_instance) {
		m_instance = new DynamicTexture();
	}
	return m_instance;
}

DynamicTexture::DynamicTexture()
	: m_preload_idx(0)
{
	m_width = WIDTH;
	m_height = HEIGHT;

	initOpenGLExtensions();

	InitTexture(m_width, m_height);
	InitFBO();
	InitRoot(m_width, m_height);
}

DynamicTexture::~DynamicTexture()
{
	glDeleteTextures(1, &m_tex);
	glDeleteFramebuffersEXT(1, &m_fbo);

	delete m_root;

	std::map<wxString, TPNode*>::iterator itr = m_map_images.begin();
	for ( ; itr != m_map_images.end(); ++itr) {
		delete itr->second;
	}
	m_map_images.clear();
}

void DynamicTexture::Begin()
{
	++m_preload_idx;
}

void DynamicTexture::Insert(const Image& img)
{
	// find
	const wxString& filepath = img.filepath();
	if (m_map_images.find(filepath) != m_map_images.end()) {
		return;
	}

	// todo need insert directly
	if (m_preload_idx != 0) {
		m_preload_list.insert(&img);
	} else {
		ShaderNew* shader = ShaderNew::Instance();
		shader->SetFBO(m_fbo);
		shader->sprite();
		glViewport(0, 0, m_width, m_height);
		InsertImage(img);
		shader->SetFBO(0);
	}
}

void DynamicTexture::End()
{
	--m_preload_idx;

	if (m_preload_idx > 0 || m_preload_list.empty()) {
		return;
	}

	ShaderNew* shader = ShaderNew::Instance();
	shader->SetFBO(m_fbo);
	shader->sprite();

	glViewport(0, 0, m_width, m_height);

	std::set<const Image*, ImageSizeCmp>::iterator itr 
		= m_preload_list.begin();
	for ( ; itr != m_preload_list.end(); ++itr) {
		InsertImage(**itr);
	}
	m_preload_list.clear();

	shader->SetFBO(0);
}

const TPNode* DynamicTexture::Query(const Image& img) const
{
	const wxString& filepath = img.filepath();
	std::map<wxString, TPNode*>::const_iterator itr 
		= m_map_images.find(filepath);
	if (itr != m_map_images.end()) {
		return itr->second;
	} else {
		return NULL;
	}
}

void DynamicTexture::DebugDraw() const
{
	ShaderNew* shader = ShaderNew::Instance();
	shader->SetFBO(0);
	shader->sprite();

	float vb[16];

	vb[0] = 0, vb[1] = 0;
	vb[2] = 0, vb[3] = 0;

	vb[4] = 0, vb[5] = 1;
	vb[6] = 0, vb[7] = 1;

	vb[8] = 1, vb[9] = 1;
	vb[10] = 1, vb[11] = 1;

	vb[12] = 1, vb[13] = 0;
	vb[14] = 1, vb[15] = 0;

	ShaderNew::Instance()->Draw(vb, m_tex);
}

void DynamicTexture::InitTexture(int width, int height)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glGenTextures(1, &m_tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_tex);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	byte* empty_data = new byte[width*height*4];
	memset(empty_data, 0, width*height*4);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei)width, (GLsizei)height, GL_RGBA, GL_UNSIGNED_BYTE, &empty_data[0]);
	delete[] empty_data;
}

void DynamicTexture::InitFBO()
{
	ShaderNew* shader = ShaderNew::Instance();

	glGenFramebuffersEXT(1, &m_fbo);
	shader->SetFBO(m_fbo);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_tex, 0);

	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
		throw Exception("Create FBO error: %d", status);
	}

	shader->SetFBO(0);
	shader->SetTexture(0);
}

void DynamicTexture::InitRoot(int width, int height)
{
	m_root = new d2d::TPNode(width, height);

	d2d::TPNode* c = new d2d::TPNode(width, height);
	m_root->SetChild(c);
}

void DynamicTexture::InsertImage(const Image& img)
{
	d2d::Rect r = img.getRegion();
	int w = r.xLength();
	int h = r.yLength();
	d2d::TPNode* n = NULL;
	if (m_root->IsRoomEnough(w, h)) {
		n = m_root->Insert(&img, w+PADDING*2, h+PADDING*2);
	} else {
		n = m_root->Insert(&img, h+PADDING*2, w+PADDING*2);
	}

	if (!n) {
		return;
	}

	m_map_images.insert(std::make_pair(img.filepath(), n));
	// draw fbo
	float xmin = ((float)n->GetMinX() / m_width) * 2 - 1;
	float xmax = ((float)n->GetMaxX() / m_width) * 2 - 1;
	float ymin = ((float)n->GetMinY() / m_height) * 2 - 1;
	float ymax = ((float)n->GetMaxY() / m_height) * 2 - 1;
	float vb[16];
	vb[0] = xmin;
	vb[1] = ymin;
	vb[4] = xmax;
	vb[5] = ymin;
	vb[8] = xmax;
	vb[9] = ymax;
	vb[12] = xmin;
	vb[13] = ymax;
	if (n->IsRotated())
	{
		vb[2] = 0;
		vb[3] = 1;
		vb[6] = 0;
		vb[7] = 0;
		vb[10] = 1;
		vb[11] = 0;
		vb[14] = 1;
		vb[15] = 1;
	}
	else
	{
		vb[2] = 0;
		vb[3] = 0;
		vb[6] = 1;
		vb[7] = 0;
		vb[10] = 1;
		vb[11] = 1;
		vb[14] = 0;
		vb[15] = 1;
	}
	ShaderNew::Instance()->Draw(vb, img.textureID());
}

}