#include "Image.h"
#include "TextureImgData.h"
#include "TextureFBO.h"

#include "render/BlendShader.h"
#include "render/ScreenFBO.h"
#include "common/Exception.h"
#include "common/Config.h"

#include <easyimage.h>
#include <wx/filename.h>

namespace d2d
{

Image::Image()
{
	m_tex = new TextureImgData;
}

Image::Image(ImageData* img_data)
{
	m_tex = new TextureImgData;
	m_tex->LoadFromMemory(img_data);

	m_ori_w = m_tex->GetWidth();
	m_ori_h = m_tex->GetHeight();
	m_offset.set(0, 0);
}

Image::Image(const FBO* fbo)
{
	m_tex = new TextureFBO(fbo);
}

Image::~Image()
{
	ImageMgr::Instance()->RemoveItem(m_tex->GetFilepath());
	delete m_tex;
}

bool Image::LoadFromFile(const std::string& filepath)
{
	if (!Config::Instance()->GetSettings().load_image) {
		m_tex->LoadFromMemory(ImageDataMgr::Instance()->GetItem(filepath));
		TextureFactory::Instance()->Load(filepath, m_ori_w, m_ori_h, m_offset);
		return true;
	}

	if (!wxFileName::FileExists(filepath)) {
		throw Exception("Image File: %s don't exist!", filepath.c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	if (Config::Instance()->GetSettings().open_image_edge_clip) {
		LoadWithClip(filepath);
	} else {
		m_tex->LoadFromFile(filepath);
		m_ori_w = m_tex->GetWidth();
		m_ori_h = m_tex->GetHeight();
	}

	if (m_tex->GetWidth() == 0 || m_tex->GetHeight() == 0) {
		return true;
	}

	// 		// todo
	// 		DynamicTexture::Instance()->Insert(this);
	if (Config::Instance()->IsUseDTex()) {
		DynamicTexAndFont::Instance()->AddImage(this);
	}

	return true;
}

void Image::ReloadTexture()
{
	m_tex->Reload();
}

std::string Image::GetFilepath() const 
{ 
	return m_tex->GetFilepath(); 
}

unsigned int Image::GetTexID() const 
{ 
	return m_tex->GetTexID(); 
}

int Image::GetChannels() const 
{ 
	return m_tex->GetChannels(); 
}

int Image::GetOriginWidth() const 
{ 
	return m_ori_w; 
}

int Image::GetOriginHeight() const 
{ 
	return m_ori_h; 
}

int Image::GetClippedWidth() const 
{ 
	return m_tex->GetWidth(); 
}

int Image::GetClippedHeight() const 
{ 
	return m_tex->GetHeight(); 
}

const uint8_t* Image::GetPixelData() const 
{ 
	return m_tex->GetPixelData(); 
}

void Image::Draw(const Matrix& mt, const ISprite* spr) const
{
	////////////////////////////////////////////////////////////////////////////
	//// 原始 直接画
 //   	ShaderMgr* shader = ShaderMgr::Instance();
 //  s 	shader->sprite();
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

	float hw = m_tex->GetWidth() * 0.5f,
		hh = m_tex->GetHeight() * 0.5f;

	float px = 0, py = 0;
	if (spr) {
		px = spr->GetPerspective().x;
		py = spr->GetPerspective().y;
	}

 	d2d::Vector vertices[4];
	vertices[0] = Vector(-hw - px, -hh - py);
	vertices[1] = Vector(hw + px, -hh + py);
	vertices[2] = Vector(hw - px, hh - py);
	vertices[3] = Vector(-hw + px, hh + py);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = Math::transVector(vertices[i] + m_offset, mt);
	}

	int texid;
	d2d::Vector texcoords[4];
	float txmin, txmax, tymin, tymax;
	DynamicTexAndFont* dt = NULL;
	const TPNode* n = NULL;
	if (Config::Instance()->IsUseDTex()) {
		dt = DynamicTexAndFont::Instance();
		n = dt->Query(m_tex->GetFilepath());
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

// 		if (texid != 1) {
// 			wxLogDebug(_T("img dt's tex = %d"), texid);
// 		}

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

		texid = m_tex->GetTexID();
	 	txmin = tymin = 0;
	 	txmax = tymax = 1;
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

const ImageData* Image::GetImageData() const 
{ 
	return m_tex->GetImageData(); 
}

void Image::LoadWithClip(const std::string& filepath)
{
	ImageData* img_data = ImageDataMgr::Instance()->GetItem(filepath);

	m_ori_w = img_data->GetWidth();
	m_ori_h = img_data->GetHeight();

	if (img_data->GetChannels() != 4) 
	{
		m_tex->LoadFromMemory(img_data);
	} 
	else 
	{
		eimage::ImageTrim trim(*img_data);
		Rect r = trim.Trim();
		if (r.xLength() >= img_data->GetWidth() && r.yLength() >= img_data->GetHeight()) {
			m_tex->LoadFromMemory(img_data);
		} else {
			int w = img_data->GetWidth(),
				h = img_data->GetHeight();

			eimage::ImageClip clip(*img_data);
			const uint8_t* c_pixels = clip.Clip(r);

			img_data->SetContent(c_pixels, r.xLength(), r.yLength());
			m_tex->LoadFromMemory(img_data);

			m_offset.x = r.xCenter() - w * 0.5f;
			m_offset.y = r.yCenter() - h * 0.5f;
		}
	}

	img_data->Release();
}

} // d2d