#include "Image.h"

#include "dataset/TPNode.h"
#include "render/GL10.h"
#include "render/Shader.h"
#include "render/ShaderMgr.h"
//#include "render/DynamicTexture.h"
#include "render/DynamicTexAndFont.h"
#include "render/RenderList.h"
#include "render/BlendShader.h"
#include "render/ScreenFBO.h"
#include "common/tools.h"
#include "common/Exception.h"
#include "common/Math.h"
#include "common/Config.h"
#include "common/SettingData.h"
#include "common/TodoConfig.h"

#include <fstream>
#include <easyimage.h>
#include <wx/filename.h>

namespace d2d
{

Image::Image(const uint8_t* pixel, int width, int height)
	: m_tex(pixel, width, height)
{
	ResetClippedRegion();
}

Image::~Image()
{
	ImageMgr::Instance()->RemoveItem(m_tex.GetFilepath());
}

bool Image::LoadFromFile(const std::string& filepath)
{
	if (!wxFileName::FileExists(filepath)) {
		throw Exception("Image File: %s don't exist!", filepath.c_str());
	}

#ifdef NOT_LOAD_IMAGE
	// todo
	m_clipped_region.xMin = m_clipped_region.xMax = m_clipped_region.yMin = m_clipped_region.yMax = 0;
	return true;
#endif

	m_tex.LoadFromFile(filepath);

 	if (m_tex.GetTexID() == 0)
 	{
//		assert(0);
//		m_width = m_height = 0;
 		return true;
 	}
 	else
 	{
		if (Config::Instance()->GetSettings().open_image_edge_clip) 
		{
			eimage::ImageTrim trim(this);
			d2d::Rect r = trim.Trim();
			if (r.isValid()) {
				r.translate(d2d::Vector(-m_tex.GetWidth()*0.5f, -m_tex.GetHeight()*0.5f));
				m_clipped_region = r;
			}
		} 
		else 
		{
			ResetClippedRegion();
		}

// 		// todo
// 		DynamicTexture::Instance()->Insert(this);
		if (Config::Instance()->IsUseDTex()) {
			DynamicTexAndFont::Instance()->AddImage(this);
		}

 		return true;
 	}
}

void Image::Reload()
{
	m_tex.Reload();
	ResetClippedRegion();
}

void Image::Draw(const Matrix& mt, const Rect& r, const ISprite* spr) const
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

	float px = 0, py = 0;
	if (spr) {
		px = spr->GetPerspective().x;
		py = spr->GetPerspective().y;
	}

 	d2d::Vector vertices[4];
 	vertices[0] = Math::transVector(Vector(r.xMin - px, r.yMin - py), mt);
 	vertices[1] = Math::transVector(Vector(r.xMax + px, r.yMin + py), mt);
 	vertices[2] = Math::transVector(Vector(r.xMax - px, r.yMax - py), mt);
 	vertices[3] = Math::transVector(Vector(r.xMin + px, r.yMax + py), mt);

	int texid;
	d2d::Vector texcoords[4];
	float txmin, txmax, tymin, tymax;
	DynamicTexAndFont* dt = NULL;
	const TPNode* n = NULL;
	if (Config::Instance()->IsUseDTex()) {
		dt = DynamicTexAndFont::Instance();
		n = dt->Query(m_tex.GetFilepath());
	}
 	if (n)
 	{
 		float extend = dt->GetExtend();
 		int width = dt->GetWidth();
 		int height = dt->GetHeight();
 		texid = dt->GetTextureID();
		txmin = (n->GetMinX()+extend) / width;
		txmax = (n->GetMaxX()-extend) / width;
		tymin = (n->GetMinY()+extend) / height;
		tymax = (n->GetMaxY()-extend) / height;

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
		//wxLogDebug("Fail to insert dtex: %s", m_filepath.c_str());

		texid = m_tex.GetTexID();
		float w = m_tex.GetWidth(),
			h = m_tex.GetHeight();
		float hw = 0.5f * w,
			hh = 0.5f * h;
	 	txmin = (r.xMin + hw) / w;
	 	txmax = (r.xMax + hw) / w;
	 	tymin = (r.yMin + hh) / h;
	 	tymax = (r.yMax + hh) / h;
	}
 	texcoords[0].set(txmin, tymin);
 	texcoords[1].set(txmax, tymin);
 	texcoords[2].set(txmax, tymax);
 	texcoords[3].set(txmin, tymax);

//	RenderList::Instance()->Insert(texid, vertices, texcoords);

	ShaderMgr* mgr = ShaderMgr::Instance();
	mgr->sprite();

	if (BlendShader* blend_shader = dynamic_cast<BlendShader*>(mgr->GetSpriteShader())) {
		SpriteRenderer* rd = SpriteRenderer::Instance();
		const Camera* cam = rd->GetCamera();
		assert(cam);

		int w = ScreenFBO::Instance()->GetFBO().GetWidth(),
			h = ScreenFBO::Instance()->GetFBO().GetHeight();

		//////////////////////////////////////////////////////////////////////////

		assert(spr);
		d2d::Vector vertices_scr[4];
		d2d::Rect r = spr->GetRect();
		vertices_scr[0] = Math::transVector(Vector(r.xMin, r.yMin), mt);
		vertices_scr[1] = Math::transVector(Vector(r.xMax, r.yMin), mt);
		vertices_scr[2] = Math::transVector(Vector(r.xMax, r.yMax), mt);
		vertices_scr[3] = Math::transVector(Vector(r.xMin, r.yMax), mt);

		d2d::Vector tex_coolds_base[4];
		for (int i = 0; i < 4; ++i) {
			tex_coolds_base[i] = cam->transPosProjectToScreen(vertices_scr[i], w, h);
			tex_coolds_base[i].y = h - 1 - tex_coolds_base[i].y;
			tex_coolds_base[i].x /= w;
			tex_coolds_base[i].y /= h;
			tex_coolds_base[i].x = std::min(std::max(0.0f, tex_coolds_base[i].x), 1.0f);
			tex_coolds_base[i].y = std::min(std::max(0.0f, tex_coolds_base[i].y), 1.0f);
		}

		//////////////////////////////////////////////////////////////////////////

//  		assert(spr);
//  		d2d::Vector vertices_scr[4];
//  		d2d::Rect r = spr->GetRect();
//  		vertices_scr[0] = Math::transVector(Vector(0.0f, 0.0f), mt);
//  		vertices_scr[1] = Math::transVector(Vector(r.xLength(), 0.0f), mt);
//  		vertices_scr[2] = Math::transVector(Vector(r.xLength(), r.yLength()), mt);
//  		vertices_scr[3] = Math::transVector(Vector(0.0f, r.yLength()), mt);
//  
//  		d2d::Vector tex_coolds_base[4];
//  		for (int i = 0; i < 4; ++i) {
//  			tex_coolds_base[i] = cam->transPosProjectToScreen(vertices_scr[i], w, h);
//  			tex_coolds_base[i].x /= w;
//  			tex_coolds_base[i].y /= h;
//  			tex_coolds_base[i].x = std::min(std::max(0.0f, tex_coolds_base[i].x), 1.0f);
//  			tex_coolds_base[i].y = std::min(std::max(0.0f, tex_coolds_base[i].y), 1.0f);
//  		}

		//////////////////////////////////////////////////////////////////////////

		blend_shader->DrawBlend(vertices, texcoords, tex_coolds_base, texid);
	} else {
		mgr->Draw(vertices, texcoords, texid);
	}
}

void Image::ResetClippedRegion()
{
	m_clipped_region.xMax = m_tex.GetWidth() * 0.5f;
	m_clipped_region.xMin = -m_clipped_region.xMax;
	m_clipped_region.yMax = m_tex.GetHeight() * 0.5f;
	m_clipped_region.yMin = -m_clipped_region.yMax;
}

} // d2d