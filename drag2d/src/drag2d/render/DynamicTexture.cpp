#include "DynamicTexture.h"

#include "common/tools.h"
#include "common/Exception.h"
#include "dataset/TPNode.h"
#include "render/ShaderNew.h"

#include <opengl/opengl.h>
#include <gl/gl.h>
#include <gl/glu.h>

namespace d2d
{

const int DynamicTexture::WIDTH = 512;
const int DynamicTexture::HEIGHT = 512;
const int DynamicTexture::PADDING = 1;
const int DynamicTexture::EXTRUDE = 1;

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
	m_padding = PADDING;
	m_extrude = EXTRUDE;

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

void DynamicTexture::Insert(Image* img)
{
	// find
	const wxString& filepath = img->filepath();
	if (m_map_images.find(filepath) != m_map_images.end()) {
		return;
	}

	// todo need insert directly
	if (m_preload_idx != 0) {
		img->retain();
		m_preload_list.push_back(img);
	} else {
		ShaderNew* shader = ShaderNew::Instance();
		shader->SetFBO(m_fbo);
 		shader->sprite();
 		glViewport(0, 0, m_width, m_height);
		InsertImage(img);
		// set fbo to force flush
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
	std::sort(m_preload_list.begin(), m_preload_list.end(), ImageSizeCmp());
	std::vector<const Image*>::iterator itr = m_preload_list.begin();
	for ( ; itr != m_preload_list.end(); ++itr) {
		const Image* img = *itr;
		InsertImage(img);
		img->release();
	}
	m_preload_list.clear();

	// set fbo to force flush
	// todo dtex连续insert会慢
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

void DynamicTexture::ReloadTexture()
{
	InitTexture(m_width, m_height, m_tex);
	InitFBO(m_fbo);

	// init content
	ShaderNew* shader = ShaderNew::Instance();
	shader->SetFBO(m_fbo);
	shader->sprite();

	glViewport(0, 0, m_width, m_height);
	std::map<wxString, TPNode*>::iterator itr = m_map_images.begin();
	for ( ; itr != m_map_images.end(); ++itr)
	{
		Image* img = ImageMgr::Instance()->getItem(itr->first);
		assert(img);
		TPNode* n = itr->second;
		assert(n);
		DrawNode(n, img);
	}

	// set fbo to force flush
	// todo dtex之后insert时，不能连续
	shader->SetFBO(0);
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

void DynamicTexture::InitTexture(int width, int height, int tex_id)
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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	byte* empty_data = new byte[width*height*4];
	memset(empty_data, 0, width*height*4);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei)width, (GLsizei)height, GL_RGBA, GL_UNSIGNED_BYTE, &empty_data[0]);
	delete[] empty_data;

	m_tex = tex_id;
}

void DynamicTexture::InitFBO(int fbo_id)
{
	ShaderNew* shader = ShaderNew::Instance();

	if (fbo_id == 0) {
		glGenFramebuffersEXT(1, (GLuint*)&fbo_id);
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo_id);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_tex, 0);

	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
		throw Exception("Create FBO error: %d", status);
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ShaderNew::Instance()->GetFboID());

	m_fbo = fbo_id;
}

void DynamicTexture::InitRoot(int width, int height)
{
	m_root = new d2d::TPNode(width, height);

	d2d::TPNode* c = new d2d::TPNode(width, height);
	m_root->SetChild(c);
}

void DynamicTexture::InsertImage(const Image* img)
{
	const wxString& filepath = img->filepath();
	if (m_map_images.find(filepath) != m_map_images.end()) {
		return;
	}

	d2d::Rect r = img->getRegion();
	int w = r.xLength();
	int h = r.yLength();
	d2d::TPNode* n = NULL;
	if (m_root->IsRoomEnough(w, h)) {
		n = m_root->Insert(img, w+m_padding*2, h+m_padding*2);
	} else {
		n = m_root->Insert(img, h+m_padding*2, w+m_padding*2);
	}

	if (!n) {
		return;
	}

	DrawNode(n, img);

	m_map_images.insert(std::make_pair(img->filepath(), n));
}

void DynamicTexture::DrawNode(const TPNode* n, const Image* img) const
{
	d2d::Rect r = img->getRegion();

	Rect r_vertex, r_texcoords;
	r_vertex.xMin = ((float)(n->GetMinX()+m_padding) / m_width) * 2 - 1;
	r_vertex.xMax = ((float)(n->GetMaxX()-m_padding) / m_width) * 2 - 1;
	r_vertex.yMin = ((float)(n->GetMinY()+m_padding) / m_height) * 2 - 1;
	r_vertex.yMax = ((float)(n->GetMaxY()-m_padding) / m_height) * 2 - 1;

	int ori_width = img->originWidth(),
		ori_height = img->originHeight();
	r_texcoords.xMin = (r.xMin + ori_width * 0.5f) / ori_width;
	r_texcoords.xMax = (r.xMax + ori_width * 0.5f) / ori_width;
	r_texcoords.yMin = (r.yMin + ori_height * 0.5f) / ori_height;
	r_texcoords.yMax = (r.yMax + ori_height * 0.5f) / ori_height;
	DrawRegion(r_vertex, r_texcoords, img->textureID(), n->IsRotated());

	DrawExtrude(img, n);
}

void DynamicTexture::DrawRegion(const Rect& vertex, const Rect& texcoords, int texid, bool is_rotate) const
{
	float vb[16];
	vb[0] = vertex.xMin;
	vb[1] = vertex.yMin;
	vb[4] = vertex.xMax;
	vb[5] = vertex.yMin;
	vb[8] = vertex.xMax;
	vb[9] = vertex.yMax;
	vb[12] = vertex.xMin;
	vb[13] = vertex.yMax;

	if (is_rotate)
	{
		vb[2] = texcoords.xMin;
		vb[3] = texcoords.yMax;
		vb[6] = texcoords.xMin;
		vb[7] = texcoords.yMin;
		vb[10] = texcoords.xMax;
		vb[11] = texcoords.yMin;
		vb[14] = texcoords.xMax;
		vb[15] = texcoords.yMax;
	}
	else
	{
		vb[2] = texcoords.xMin;
		vb[3] = texcoords.yMin;
		vb[6] = texcoords.xMax;
		vb[7] = texcoords.yMin;
		vb[10] = texcoords.xMax;
		vb[11] = texcoords.yMax;
		vb[14] = texcoords.xMin;
		vb[15] = texcoords.yMax;
	}
	ShaderNew::Instance()->Draw(vb, texid);
}

void DynamicTexture::DrawExtrude(const Image* img, const TPNode* n) const
{
	d2d::Rect r = img->getRegion();
	int ori_width = img->originWidth(),
		ori_height = img->originHeight();
	Rect r_vertex, r_texcoords;

	// up
	r_texcoords.xMin = (r.xMin + ori_width * 0.5f) / ori_width;
	r_texcoords.xMax = (r.xMax + ori_width * 0.5f) / ori_width;
	r_texcoords.yMin = (r.yMax - 1 + ori_height * 0.5f) / ori_height;
	r_texcoords.yMax = (r.yMax + ori_height * 0.5f) / ori_height;
	r_vertex.xMin = ((float)(n->GetMinX()+m_padding) / m_width) * 2 - 1;
	r_vertex.xMax = ((float)(n->GetMaxX()-m_padding) / m_width) * 2 - 1;
	for (int i = 0; i < m_extrude; ++i)
	{
		r_vertex.yMin = ((float)(n->GetMaxY()+i-m_padding) / m_height) * 2 - 1;
		r_vertex.yMax = ((float)(n->GetMaxY()+1+i-m_padding) / m_height) * 2 - 1;
		DrawRegion(r_vertex, r_texcoords, img->textureID(), n->IsRotated());
	}

	// down
	r_texcoords.yMin = (r.yMin + ori_height * 0.5f) / ori_height;
	r_texcoords.yMax = (r.yMin + 1 + ori_height * 0.5f) / ori_height;
	for (int i = 0; i < m_extrude; ++i)
	{
		r_vertex.yMin = ((float)(n->GetMinY()-1-i+m_padding) / m_height) * 2 - 1;
		r_vertex.yMax = ((float)(n->GetMinY()-i+m_padding) / m_height) * 2 - 1;
		DrawRegion(r_vertex, r_texcoords, img->textureID(), n->IsRotated());
	}

	// left
	r_texcoords.xMin = (r.xMin + ori_width * 0.5f) / ori_width;
	r_texcoords.xMax = (r.xMin + 1 + ori_width * 0.5f) / ori_width;
	r_texcoords.yMin = (r.yMin + ori_height * 0.5f) / ori_height;
	r_texcoords.yMax = (r.yMax + ori_height * 0.5f) / ori_height;
	r_vertex.yMin = ((float)(n->GetMinY()+m_padding) / m_height) * 2 - 1;
	r_vertex.yMax = ((float)(n->GetMaxY()-m_padding) / m_height) * 2 - 1;
	for (int i = 0; i < m_extrude; ++i)
	{
		r_vertex.xMin = ((float)(n->GetMinX()-1-i+m_padding) / m_width) * 2 - 1;
		r_vertex.xMax = ((float)(n->GetMinX()-i+m_padding) / m_width) * 2 - 1;
		DrawRegion(r_vertex, r_texcoords, img->textureID(), n->IsRotated());
	}

	// right
	r_texcoords.xMin = (r.xMax - 1 + ori_width * 0.5f) / ori_width;
	r_texcoords.xMax = (r.xMax + ori_width * 0.5f) / ori_width;
	for (int i = 0; i < m_extrude; ++i)
	{
		r_vertex.xMin = ((float)(n->GetMaxX()+i-m_padding) / m_width) * 2 - 1;
		r_vertex.xMax = ((float)(n->GetMaxX()+1+i-m_padding) / m_width) * 2 - 1;
		DrawRegion(r_vertex, r_texcoords, img->textureID(), n->IsRotated());
	}
}

}