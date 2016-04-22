#include "dev_config.h"

#include "Vector3D.h"
#include "Image.h"
#include "TextureImgData.h"
#include "Exception.h"
#include "Config.h"
#include "TextureFBO.h"
#ifdef OPEN_SCREEN_CACHE
#include "render/ScreenCache.h"
#endif // OPEN_SCREEN_CACHE
//#include "BlendShader.h"
#include "EE_DTex.h"
#include "SettingData.h"
#include "ImageData.h"
#include "TextureFactory.h"
#include "FileHelper.h"
#include "Sprite.h"
#include "Math2D.h"
#include "SpriteRenderer.h"
#include "ScreenCache.h"
#include "Camera.h"
#include "CameraMgr.h"
#include "Pseudo3DCamera.h"
#include "ImageClip.h"
#include "ImageTrim.h"
#include "RenderParams.h"
#include "trans_color.h"

#include <shaderlab.h>

namespace ee
{

Image::Image()
{
	m_tex = new TextureImgData;
}

Image::Image(ImageData* img_data)
{
	m_tex = new TextureImgData;
	m_tex->LoadFromMemory(img_data);

	m_ori_w = static_cast<float>(m_tex->GetWidth());
	m_ori_h = static_cast<float>(m_tex->GetHeight());
	m_offset.Set(0, 0);
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

	if (!FileHelper::IsFileExist(filepath)) {
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

	if (Config::Instance()->IsUseDTex() && CanUseDTex()) {
		DTex* dcb = DTex::Instance();
		dcb->LoadBegin();
		dcb->Load(this);
		dcb->LoadEnd();
	}

	return true;
}

void Image::ReloadTexture()
{
	m_tex->Reload();

	if (Config::Instance()->IsUseDTex() && CanUseDTex()) {
		DTex::Instance()->Reload(this);
	}
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

void Image::Draw(const RenderParams& trans, const Sprite* spr, 
				 const Sprite* root) const
{
	float hw = m_tex->GetWidth() * 0.5f,
		  hh = m_tex->GetHeight() * 0.5f;

	float px = 0, py = 0;
	if (spr) {
		px = spr->GetPerspective().x;
		py = spr->GetPerspective().y;
	}

	Vector vertices[4];
	vertices[0] = Vector(-hw - px, -hh - py);
	vertices[1] = Vector( hw + px, -hh + py);
	vertices[2] = Vector( hw - px,  hh - py);
	vertices[3] = Vector(-hw + px,  hh + py);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = Math2D::TransVector(vertices[i] + m_offset, trans.mt);
	}

	int texid;
	Vector texcoords[4];

	float* c2_texcoords = NULL;
	if (Config::Instance()->IsUseDTex() && CanUseDTex()) {
		c2_texcoords = DTex::Instance()->Query(this, &texid);
	}
	if (c2_texcoords)
	{
		texcoords[0].x = c2_texcoords[0];
		texcoords[0].y = c2_texcoords[1];
		texcoords[1].x = c2_texcoords[2];
		texcoords[1].y = c2_texcoords[3];
		texcoords[2].x = c2_texcoords[4];
		texcoords[2].y = c2_texcoords[5];
		texcoords[3].x = c2_texcoords[6];
		texcoords[3].y = c2_texcoords[7];
	}
	else
	{
		texid = m_tex->GetTexID();

		float txmin, txmax, tymin, tymax;
	 	txmin = tymin = 0;
	 	txmax = tymax = 1;

		texcoords[0].Set(txmin, tymin);
		texcoords[1].Set(txmax, tymin);
		texcoords[2].Set(txmax, tymax);
		texcoords[3].Set(txmin, tymax);
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	if (mgr->GetShaderType() == sl::BLEND) 
	{
		sl::BlendShader* shader = static_cast<sl::BlendShader*>(mgr->GetShader(sl::BLEND));
		shader->SetColor(ee::color2int(trans.color.multi, ee::PT_ABGR),
			ee::color2int(trans.color.add, ee::PT_ABGR));
		shader->SetColorMap(ee::color2int(trans.color.r, ee::PT_ABGR),
			ee::color2int(trans.color.g, ee::PT_ABGR),
			ee::color2int(trans.color.b, ee::PT_ABGR));

		assert(spr);

		if (root) {
			Vector offset = root->GetPosition();
			for (int i = 0; i < 4; ++i) {
				vertices[i] -= offset;
			}
		}

		Vector vertices_scr[4];
		float img_hw = m_tex->GetWidth() * 0.5f,
			  img_hh = m_tex->GetHeight() * 0.5f;
 		vertices_scr[0] = Math2D::TransVector(Vector(-img_hw, -img_hh), trans.mt);
 		vertices_scr[1] = Math2D::TransVector(Vector( img_hw, -img_hh), trans.mt);
 		vertices_scr[2] = Math2D::TransVector(Vector( img_hw,  img_hh), trans.mt);
 		vertices_scr[3] = Math2D::TransVector(Vector(-img_hw,  img_hh), trans.mt);

		Vector tex_coords_base[4];
		SpriteRenderer* rd = SpriteRenderer::Instance();
		const Camera* cam = CameraMgr::Instance()->GetCamera();
		assert(cam);
		int w, h;
		ScreenCache::Instance()->GetSize(w, h);
		for (int i = 0; i < 4; ++i) {
			tex_coords_base[i] = cam->TransPosProjectToScreen(vertices_scr[i], w, h);
			tex_coords_base[i].y = h - 1 - tex_coords_base[i].y;
			tex_coords_base[i].x /= w;
			tex_coords_base[i].y /= h;
		}
		shader->Draw(&vertices[0].x, &texcoords[0].x, &tex_coords_base[0].x, texid, ScreenCache::Instance()->GetTexID());
	}
	else
	{
		const Camera* cam = CameraMgr::Instance()->GetCamera();
		if (cam->Type() == "ortho") 
		{
// 			sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
// 			shader->SetColor(trans.color);
			if (mgr->GetShaderType() == sl::FILTER) {
				sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
				shader->Draw(&vertices[0].x, &texcoords[0].x, texid);
			} else if (mgr->GetShaderType() == sl::SPRITE2) {
				sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
				shader->Draw(&vertices[0].x, &texcoords[0].x, texid);
			}
		} 
		else
		{
			assert(mgr->GetShaderType() == sl::SPRITE3);
			const Pseudo3DCamera* pcam = static_cast<const Pseudo3DCamera*>(cam);
			float z[4];
			trans.camera.CalculateZ(pcam, vertices, z);

			std::vector<vec3> _vertices;
			_vertices.push_back(vec3(vertices[0].x, vertices[0].y, z[0]));
			_vertices.push_back(vec3(vertices[1].x, vertices[1].y, z[1]));
			_vertices.push_back(vec3(vertices[2].x, vertices[2].y, z[2]));
			_vertices.push_back(vec3(vertices[0].x, vertices[0].y, z[0]));
			_vertices.push_back(vec3(vertices[2].x, vertices[2].y, z[2]));
			_vertices.push_back(vec3(vertices[3].x, vertices[3].y, z[3]));

			std::vector<Vector> _texcoords;
			_texcoords.push_back(texcoords[0]);
			_texcoords.push_back(texcoords[1]);
			_texcoords.push_back(texcoords[2]);
			_texcoords.push_back(texcoords[0]);
			_texcoords.push_back(texcoords[2]);
			_texcoords.push_back(texcoords[3]);

			mgr->SetShader(sl::SPRITE3);
			sl::Sprite3Shader* shader = static_cast<sl::Sprite3Shader*>(mgr->GetShader(sl::SPRITE3));
			shader->SetColor(ee::color2int(trans.color.multi, ee::PT_ABGR),
				ee::color2int(trans.color.add, ee::PT_ABGR));
			shader->SetColorMap(ee::color2int(trans.color.r, ee::PT_ABGR),
				ee::color2int(trans.color.g, ee::PT_ABGR),
				ee::color2int(trans.color.b, ee::PT_ABGR));
			shader->Draw(&_vertices[0].x, &_texcoords[0].x, texid);
		}
	}
}

void Image::InvalidRect(const Matrix& mt) const
{
#ifdef OPEN_SCREEN_CACHE
	float hw = m_tex->GetWidth() * 0.5f,
		  hh = m_tex->GetHeight() * 0.5f;
	Vector vertices[4];
	vertices[0] = Vector(-hw, -hh);
	vertices[1] = Vector( hw, -hh);
	vertices[2] = Vector( hw,  hh);
	vertices[3] = Vector(-hw,  hh);

	float xmin = FLT_MAX, ymin = FLT_MAX, xmax = -FLT_MAX, ymax = -FLT_MAX;
	for (int i = 0; i < 4; ++i) {
		Vector pos = Math2D::TransVector(vertices[i] + m_offset, mt);
		if (pos.x < xmin) xmin = pos.x;
		if (pos.x > xmax) xmax = pos.x;
		if (pos.y < ymin) ymin = pos.y;
		if (pos.y > ymax) ymax = pos.y;
	}
	ScreenCache::Instance()->InvalidRect(xmin, ymin, xmax, ymax);
#endif // OPEN_SCREEN_CACHE
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
		ImageTrim trim(*img_data);
		Rect r = trim.Trim();
		if (r.Width() >= img_data->GetWidth() && r.Height() >= img_data->GetHeight()) {
			m_tex->LoadFromMemory(img_data);
		} else {
			int w = img_data->GetWidth(),
				h = img_data->GetHeight();

			ImageClip clip(*img_data);
			const uint8_t* c_pixels = clip.Clip(r);

			img_data->SetContent(c_pixels, r.Width(), r.Height());
			m_tex->LoadFromMemory(img_data);

			m_offset.x = r.CenterX() - w * 0.5f;
			m_offset.y = r.CenterY() - h * 0.5f;
		}
	}

	img_data->Release();
}

bool Image::CanUseDTex() const
{
	return m_tex->GetWidth() < 512 && m_tex->GetHeight() < 512;
}

}