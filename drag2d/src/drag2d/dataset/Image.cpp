#include "Image.h"
#include "ImageLoader.h"

#include "dataset/TPNode.h"
#include "render/GL10.h"
#include "render/Shader.h"
#include "render/ShaderMgr.h"
//#include "render/DynamicTexture.h"
#include "render/DynamicTexAndFont.h"
#include "render/RenderList.h"
#include "common/tools.h"
#include "common/Exception.h"
#include "common/Math.h"
#include "common/Config.h"
#include "common/SettingData.h"
#include "common/TodoConfig.h"

#include <fstream>
#include <SOIL/SOIL.h>
#include <easyimage.h>
#include <wx/filename.h>

//#define USE_SOIL

namespace d2d
{

Image::Image()
	: m_pixels(NULL)
{
	m_textureID = 0;
	m_width = m_height = 0;
}

Image::~Image()
{
	ImageMgr::Instance()->removeItem(m_filepath);
	delete m_pixels;
}

bool Image::loadFromFile(const wxString& filepath)
{
	if (filepath.Contains("1107_1") == 0) {
		int zz = 0;
	}

	if (!wxFileName::FileExists(filepath)) {
		throw Exception("File: %s don't exist!", filepath.ToStdString().c_str());
	}

	m_filepath = filepath;

#ifdef NOT_LOAD_IMAGE
	// todo
	m_region.xMin = m_region.xMax = m_region.yMin = m_region.yMax = 0;
	return true;
#endif

 	reload(); 
 	if (m_textureID == 0)
 	{
//		assert(0);
//		m_width = m_height = 0;
 		return true;
 	}
 	else
 	{
#ifdef USE_SOIL
  		GL10::BindTexture(GL10::GL_TEXTURE_2D, m_textureID);
  		GL10::GetTexLevelParameteriv(GL10::GL_TEXTURE_2D, 0, GL10::GL_TEXTURE_WIDTH, &m_width);
  		GL10::GetTexLevelParameteriv(GL10::GL_TEXTURE_2D, 0, GL10::GL_TEXTURE_HEIGHT, &m_height);
  		GL10::BindTexture(GL10::GL_TEXTURE_2D, NULL);
#endif
		if (Config::Instance()->GetSettings().open_image_edge_clip) 
		{
			eimage::ImageProcessor processor(this);
			d2d::Rect r = processor.trim();
			if (r.isValid()) {
				r.translate(d2d::Vector(-m_width*0.5f, -m_height*0.5f));
				m_region = r;
			}
		} 
		else 
		{
			m_region.xMin = - m_width * 0.5f;
			m_region.xMax =   m_width * 0.5f;
			m_region.yMin = - m_height * 0.5f;
			m_region.yMax =   m_height * 0.5f;
		}

// 		// todo
// 		DynamicTexture::Instance()->Insert(this);
		if (Config::Instance()->IsUseDTex()) {
			DynamicTexAndFont::Instance()->AddImage(this);
		}

 		return true;
 	}
}

void Image::reload()
{
#ifdef USE_SOIL
 	m_textureID = SOIL_load_OGL_texture
 		(
 		m_filepath.c_str(),
 		SOIL_LOAD_AUTO,
 		m_textureID,
 		SOIL_FLAG_INVERT_Y
 		);
#else
	m_pixels = ImageLoader::loadTexture(m_filepath.ToStdString(), m_width, m_height, m_textureID, m_channels);

 	m_region.xMin = - m_width * 0.5f;
 	m_region.xMax =   m_width * 0.5f;
 	m_region.yMin = - m_height * 0.5f;
 	m_region.yMax =   m_height * 0.5f;

#endif
}

void Image::draw(const Matrix& mt, const Rect& r) const
{
	////////////////////////////////////////////////////////////////////////////
	//// 原始 直接画
 //   	ShaderMgr* shader = ShaderMgr::Instance();
 //   	shader->sprite();
 //   
 //   	float tot_hw = m_width * 0.5f,
 //   		  tot_hh = m_height * 0.5f;
 //   	float txmin = (r.xMin + tot_hw) / m_width,
 //   		txmax = (r.xMax + tot_hw) / m_width,
 //   		tymin = (r.yMin + tot_hh) / m_height,
 //   		tymax = (r.yMax + tot_hh) / m_height;
 //   
 // 	d2d::Vector vertices[4];
 // 	vertices[0] = Math::transVector(Vector(r.xMin, r.yMin), mt);
 // 	vertices[1] = Math::transVector(Vector(r.xMax, r.yMin), mt);
 // 	vertices[2] = Math::transVector(Vector(r.xMax, r.yMax), mt);
 // 	vertices[3] = Math::transVector(Vector(r.xMin, r.yMax), mt);
 // 
 // 	for (int i = 0; i < 4; ++i) {
 // 		scr.TransPosForRender(vertices[i]);
 // 	}
 // 
 // 	float vb[16];
 // 	vb[0] = vertices[0].x;
 // 	vb[1] = vertices[0].y;
 // 	vb[2] = txmin;
 // 	vb[3] = tymin;
 // 	vb[4] = vertices[1].x;
 // 	vb[5] = vertices[1].y;
 // 	vb[6] = txmax;
 // 	vb[7] = tymin;
 // 	vb[8] = vertices[2].x;
 // 	vb[9] = vertices[2].y;
 // 	vb[10] = txmax;
 // 	vb[11] = tymax;
 // 	vb[12] = vertices[3].x;
 // 	vb[13] = vertices[3].y;
 // 	vb[14] = txmin;
 // 	vb[15] = tymax;
 // 
 // 	shader->Draw(vb, m_textureID);

	//////////////////////////////////////////////////////////////////////////
//	用renderlist
// 	d2d::Vector vertices[4];
// 	vertices[0] = Math::transVector(Vector(r.xMin, r.yMin), mt);
// 	vertices[1] = Math::transVector(Vector(r.xMax, r.yMin), mt);
// 	vertices[2] = Math::transVector(Vector(r.xMax, r.yMax), mt);
// 	vertices[3] = Math::transVector(Vector(r.xMin, r.yMax), mt);
// 	for (int i = 0; i < 4; ++i) {
// 		scr.TransPosForRender(vertices[i]);
// 	}
// 
// 	d2d::Vector texcoords[4];
// 	float tot_hw = m_width * 0.5f,
// 		tot_hh = m_height * 0.5f;
// 	float txmin = (r.xMin + tot_hw) / m_width,
// 		txmax = (r.xMax + tot_hw) / m_width,
// 		tymin = (r.yMin + tot_hh) / m_height,
// 		tymax = (r.yMax + tot_hh) / m_height;
// 	texcoords[0].set(txmin, tymin);
// 	texcoords[1].set(txmax, tymin);
// 	texcoords[2].set(txmax, tymax);
// 	texcoords[3].set(txmin, tymax);
// 
// 	RenderList::Instance()->Insert(m_textureID, vertices, texcoords);

	//////////////////////////////////////////////////////////////////////////
	// 用dtex
 	d2d::Vector vertices[4];
 	vertices[0] = Math::transVector(Vector(r.xMin, r.yMin), mt);
 	vertices[1] = Math::transVector(Vector(r.xMax, r.yMin), mt);
 	vertices[2] = Math::transVector(Vector(r.xMax, r.yMax), mt);
 	vertices[3] = Math::transVector(Vector(r.xMin, r.yMax), mt);

	int texid;
	d2d::Vector texcoords[4];
	float txmin, txmax, tymin, tymax;
	DynamicTexAndFont* dt = NULL;
	const TPNode* n = NULL;
	if (Config::Instance()->IsUseDTex()) {
		dt = DynamicTexAndFont::Instance();
		n = dt->Query(m_filepath);
	}
 	if (n)
 	{
 		float padding = dt->GetPadding();
 		int width = dt->GetWidth();
 		int height = dt->GetHeight();
 		texid = dt->GetTextureID();
 		txmin = (n->GetMinX()+padding+0.5f) / width;
 		txmax = (n->GetMaxX()-padding-0.5f) / width;
 		tymin = (n->GetMinY()+padding+0.5f) / height;
 		tymax = (n->GetMaxY()-padding-0.5f) / height;
 
		if (texid != 1) {
			wxLogDebug(_T("img dt's tex = %d"), texid);
		}

 		if (n->IsRotated())
 		{
 			d2d::Vector tmp = vertices[3];
 			vertices[3] = vertices[2];
 			vertices[2] = vertices[1];
 			vertices[1] = vertices[0];
 			vertices[0] = tmp;
 		}
 	}
 	else
	{
		wxLogDebug("Fail to insert dtex: %s", m_filepath.c_str());

		texid = m_textureID;
	 	d2d::Vector texcoords[4];
	 	float tot_hw = m_width * 0.5f,
	 		tot_hh = m_height * 0.5f;
	 	txmin = (r.xMin + tot_hw) / m_width;
	 	txmax = (r.xMax + tot_hw) / m_width;
	 	tymin = (r.yMin + tot_hh) / m_height;
	 	tymax = (r.yMax + tot_hh) / m_height;
	}
 	texcoords[0].set(txmin, tymin);
 	texcoords[1].set(txmax, tymin);
 	texcoords[2].set(txmax, tymax);
 	texcoords[3].set(txmin, tymax);

//	RenderList::Instance()->Insert(texid, vertices, texcoords);

	ShaderMgr* shader = ShaderMgr::Instance();
	shader->sprite();
	shader->Draw(vertices, texcoords, texid);
}

} // d2d