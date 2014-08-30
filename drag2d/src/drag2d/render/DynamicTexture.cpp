#include "DynamicTexture.h"
#include "DynamicUtils.h"

#include "common/tools.h"
#include "common/Exception.h"
#include "dataset/TPNode.h"
#include "render/ShaderNew.h"
#include "render/PrimitiveDraw.h"
#include "render/style_config.h"
#include "view/Screen.h"

#include <opengl/opengl.h>
#include <gl/gl.h>
#include <gl/glu.h>

namespace d2d
{

const int DynamicTexture::WIDTH = 4096;
const int DynamicTexture::HEIGHT = 4096;
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

	m_tex = DynamicUtils::InitTexture(m_width, m_height);
	m_fbo = DynamicUtils::InitFBO(m_tex);
	m_root = DynamicUtils::InitRoot(m_width, m_height);
}

DynamicTexture::~DynamicTexture()
{
	glDeleteTextures(1, &m_tex);
	glDeleteFramebuffersEXT(1, &m_fbo);

	delete m_root;
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

void DynamicTexture::Remove(Image* img)
{
	const wxString& filepath = img->filepath();
	std::map<wxString, TPNode*>::const_iterator itr 
		= m_map_images.find(filepath);
	if (itr != m_map_images.end()) {
		itr->second->Clear();
		m_map_images.erase(itr);
	}
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
	m_tex = DynamicUtils::InitTexture(m_width, m_height, m_tex);
	m_fbo = DynamicUtils::InitFBO(m_tex, m_fbo);

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
	DynamicUtils::DebugDraw(m_tex);
}

void DynamicTexture::DebugDraw(const Screen& screen) const
{
	DynamicUtils::DebugDraw(screen, m_tex);
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
	d2d::TPNode* n = m_root->Insert(w+m_padding*2, h+m_padding*2);
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

	if (n->IsRotated())
	{
		// up
		r_texcoords.xMin = (r.xMax - 1 + ori_width * 0.5f) / ori_width;
		r_texcoords.xMax = (r.xMax + ori_width * 0.5f) / ori_width;
		r_texcoords.yMin = (r.yMin + ori_height * 0.5f) / ori_height;
		r_texcoords.yMax = (r.yMax + ori_height * 0.5f) / ori_height;
		r_vertex.xMin = ((float)(n->GetMinX()+m_padding) / m_width) * 2 - 1;
		r_vertex.xMax = ((float)(n->GetMaxX()-m_padding) / m_width) * 2 - 1;
		for (int i = 0; i < m_extrude; ++i)
		{
			r_vertex.yMin = ((float)(n->GetMaxY()+i-m_padding) / m_height) * 2 - 1;
			r_vertex.yMax = ((float)(n->GetMaxY()+1+i-m_padding) / m_height) * 2 - 1;
			DrawRegion(r_vertex, r_texcoords, img->textureID(), true);
		}

		// down
		r_texcoords.xMin = (r.xMin + ori_width * 0.5f) / ori_width;
		r_texcoords.xMax = (r.xMin + 1 + ori_width * 0.5f) / ori_width;
		for (int i = 0; i < m_extrude; ++i)
		{
			r_vertex.yMin = ((float)(n->GetMinY()-1-i+m_padding) / m_height) * 2 - 1;
			r_vertex.yMax = ((float)(n->GetMinY()-i+m_padding) / m_height) * 2 - 1;
			DrawRegion(r_vertex, r_texcoords, img->textureID(), true);
		}

		// left
		r_texcoords.xMin = (r.xMin + ori_width * 0.5f) / ori_width;
		r_texcoords.xMax = (r.xMax + ori_width * 0.5f) / ori_width;
		r_texcoords.yMin = (r.yMax - 1 + ori_height * 0.5f) / ori_height;
		r_texcoords.yMax = (r.yMax + ori_height * 0.5f) / ori_height;
		r_vertex.yMin = ((float)(n->GetMinY()+m_padding) / m_height) * 2 - 1;
		r_vertex.yMax = ((float)(n->GetMaxY()-m_padding) / m_height) * 2 - 1;
		for (int i = 0; i < m_extrude; ++i)
		{
			r_vertex.xMin = ((float)(n->GetMinX()-1-i+m_padding) / m_width) * 2 - 1;
			r_vertex.xMax = ((float)(n->GetMinX()-i+m_padding) / m_width) * 2 - 1;
			DrawRegion(r_vertex, r_texcoords, img->textureID(), true);
		}

		// right
		r_texcoords.yMin = (r.yMin + ori_height * 0.5f) / ori_height;
		r_texcoords.yMax = (r.yMin + 1 + ori_height * 0.5f) / ori_height;
		for (int i = 0; i < m_extrude; ++i)
		{
			r_vertex.xMin = ((float)(n->GetMaxX()+i-m_padding) / m_width) * 2 - 1;
			r_vertex.xMax = ((float)(n->GetMaxX()+1+i-m_padding) / m_width) * 2 - 1;
			DrawRegion(r_vertex, r_texcoords, img->textureID(), true);
		}		

		// up-left
		r_texcoords.xMin = (r.xMin + ori_width * 0.5f) / ori_width;
		r_texcoords.xMax = (r.xMin + ori_width * 0.5f + 1) / ori_width;
		r_texcoords.yMin = (r.yMax + ori_height * 0.5f - 1) / ori_height;
		r_texcoords.yMax = (r.yMax + ori_height * 0.5f) / ori_height;
		r_vertex.xMin = ((float)(n->GetMinX()+m_padding-1) / m_width) * 2 - 1;
		r_vertex.xMax = ((float)(n->GetMinX()+m_padding) / m_width) * 2 - 1;		
		r_vertex.yMin = ((float)(n->GetMinY()+m_padding-1) / m_height) * 2 - 1;
		r_vertex.yMax = ((float)(n->GetMinY()+m_padding) / m_height) * 2 - 1;		
		DrawRegion(r_vertex, r_texcoords, img->textureID(), false);

		// up-right
		r_texcoords.xMin = (r.xMax + ori_width * 0.5f - 1) / ori_width;
		r_texcoords.xMax = (r.xMax + ori_width * 0.5f) / ori_width;
		r_vertex.yMin = ((float)(n->GetMaxY()-m_padding) / m_height) * 2 - 1;
		r_vertex.yMax = ((float)(n->GetMaxY()-m_padding+1) / m_height) * 2 - 1;
		DrawRegion(r_vertex, r_texcoords, img->textureID(), false);

		// down-right
		r_texcoords.yMin = (r.yMin + ori_height * 0.5f) / ori_height;
		r_texcoords.yMax = (r.yMin + ori_height * 0.5f + 1) / ori_height;
		r_vertex.xMin = ((float)(n->GetMaxX()-m_padding) / m_width) * 2 - 1;
		r_vertex.xMax = ((float)(n->GetMaxX()-m_padding+1) / m_width) * 2 - 1;		
		DrawRegion(r_vertex, r_texcoords, img->textureID(), false);

		// down-left
		r_texcoords.xMin = (r.xMin + ori_width * 0.5f) / ori_width;
		r_texcoords.xMax = (r.xMin + ori_width * 0.5f + 1) / ori_width;
		r_vertex.yMin = ((float)(n->GetMinY()+m_padding-1) / m_height) * 2 - 1;
		r_vertex.yMax = ((float)(n->GetMinY()+m_padding) / m_height) * 2 - 1;		
		DrawRegion(r_vertex, r_texcoords, img->textureID(), false);
	}
	else
	{
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
			DrawRegion(r_vertex, r_texcoords, img->textureID(), false);
		}

		// down
		r_texcoords.yMin = (r.yMin + ori_height * 0.5f) / ori_height;
		r_texcoords.yMax = (r.yMin + 1 + ori_height * 0.5f) / ori_height;
		for (int i = 0; i < m_extrude; ++i)
		{
			r_vertex.yMin = ((float)(n->GetMinY()-1-i+m_padding) / m_height) * 2 - 1;
			r_vertex.yMax = ((float)(n->GetMinY()-i+m_padding) / m_height) * 2 - 1;
			DrawRegion(r_vertex, r_texcoords, img->textureID(), false);
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
			DrawRegion(r_vertex, r_texcoords, img->textureID(), false);
		}

		// right
		r_texcoords.xMin = (r.xMax - 1 + ori_width * 0.5f) / ori_width;
		r_texcoords.xMax = (r.xMax + ori_width * 0.5f) / ori_width;
		for (int i = 0; i < m_extrude; ++i)
		{
			r_vertex.xMin = ((float)(n->GetMaxX()+i-m_padding) / m_width) * 2 - 1;
			r_vertex.xMax = ((float)(n->GetMaxX()+1+i-m_padding) / m_width) * 2 - 1;
			DrawRegion(r_vertex, r_texcoords, img->textureID(), false);
		}

		// up-left
		r_texcoords.xMin = (r.xMin + ori_width * 0.5f) / ori_width;
		r_texcoords.xMax = (r.xMin + ori_width * 0.5f + 1) / ori_width;
		r_texcoords.yMin = (r.yMax + ori_height * 0.5f - 1) / ori_height;
		r_texcoords.yMax = (r.yMax + ori_height * 0.5f) / ori_height;
		r_vertex.xMin = ((float)(n->GetMinX()+m_padding-1) / m_width) * 2 - 1;
		r_vertex.xMax = ((float)(n->GetMinX()+m_padding) / m_width) * 2 - 1;
		r_vertex.yMin = ((float)(n->GetMaxY()-m_padding) / m_height) * 2 - 1;
		r_vertex.yMax = ((float)(n->GetMaxY()-m_padding+1) / m_height) * 2 - 1;
		DrawRegion(r_vertex, r_texcoords, img->textureID(), false);

		// up-right
		r_texcoords.xMin = (r.xMax + ori_width * 0.5f - 1) / ori_width;
		r_texcoords.xMax = (r.xMax + ori_width * 0.5f) / ori_width;
		r_vertex.xMin = ((float)(n->GetMaxX()-m_padding) / m_width) * 2 - 1;
		r_vertex.xMax = ((float)(n->GetMaxX()-m_padding+1) / m_width) * 2 - 1;
		DrawRegion(r_vertex, r_texcoords, img->textureID(), false);

		// down-right
		r_texcoords.yMin = (r.yMin + ori_height * 0.5f) / ori_height;
		r_texcoords.yMax = (r.yMin + ori_height * 0.5f + 1) / ori_height;
		r_vertex.yMin = ((float)(n->GetMinY()+m_padding-1) / m_height) * 2 - 1;
		r_vertex.yMax = ((float)(n->GetMinY()+m_padding) / m_height) * 2 - 1;
		DrawRegion(r_vertex, r_texcoords, img->textureID(), false);

		// down-left
		r_texcoords.xMin = (r.xMin + ori_width * 0.5f) / ori_width;
		r_texcoords.xMax = (r.xMin + ori_width * 0.5f + 1) / ori_width;
		r_vertex.xMin = ((float)(n->GetMinX()+m_padding-1) / m_width) * 2 - 1;
		r_vertex.xMax = ((float)(n->GetMinX()+m_padding) / m_width) * 2 - 1;
		DrawRegion(r_vertex, r_texcoords, img->textureID(), false);
	}
}

}