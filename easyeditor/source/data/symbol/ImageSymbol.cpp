#include "ImageSymbol.h"
#include "Bitmap.h"
#include "Image.h"
#include "Sprite.h"
#include "ImageData.h"
#include "EE_RVG.h"
#include "CameraMgr.h"
#include "ScreenCache.h"
#include "Camera.h"
#include "Pseudo3DCamera.h"

#include <sprite2/ImageSymbol.h>

namespace ee
{

static inline void
query_texcoords(void* ud, float* texcoords, int* texid) {
	ImageSymbol* sym = static_cast<ImageSymbol*>(ud);
	sym->GetImage()->QueryTexcoords(texcoords, texid);	
}

static inline void
proj2screen(float px, float py, int w, int h, float* sx, float* sy) {
	const Camera* cam = CameraMgr::Instance()->GetCamera();
	assert(cam);
	sm::vec2 v = cam->TransPosProjectToScreen(sm::vec2(px, py), w, h);
	*sx = v.x;
	*sy = v.y;
}

static inline bool
is_ortho_cam() {
	const Camera* cam = CameraMgr::Instance()->GetCamera();
	assert(cam);
	return cam->Type() == "ortho";
}

static inline void 
get_screen_size(int* w, int* h) {
	ScreenCache::Instance()->GetSize(*w, *h);
}

static inline float 
get_p3d_cam_angle() {
	const Camera* cam = CameraMgr::Instance()->GetCamera();
	const Pseudo3DCamera* pcam = static_cast<const Pseudo3DCamera*>(cam);
	return pcam->GetAngle();
}

static inline int 
get_screen_cache_texid() {
	return ScreenCache::Instance()->GetTexID();
}

ImageSymbol::ImageSymbol()
	: m_image(NULL)
{
	m_core = new s2::ImageSymbol(this, query_texcoords, proj2screen, is_ortho_cam, get_screen_size, get_p3d_cam_angle, get_screen_cache_texid);
}

ImageSymbol::ImageSymbol(Image* image, const std::string& filename)
	: m_image(image)
{
	m_image->Retain();
	m_core = new s2::ImageSymbol(this, query_texcoords, proj2screen, is_ortho_cam, get_screen_size, get_p3d_cam_angle, get_screen_cache_texid);
	InitCoreTex();

	m_filepath = filename;
}

ImageSymbol::~ImageSymbol()
{
	delete m_core;

	if (m_bitmap) {
		m_bitmap->Release();
	}
	if (m_image) {
		m_image->Release();
	}
}

ImageSymbol* ImageSymbol::Clone() const
{
	return NULL;
}

void ImageSymbol::Draw(const s2::RenderParams& params, const Sprite* spr) const
{
	m_core->Draw(params, spr ? spr->GetCore() : NULL);
}

void ImageSymbol::ReloadTexture() const
{
	m_image->ReloadTexture();
}

sm::rect ImageSymbol::GetSize(const Sprite* sprite/* = NULL*/) const
{
	return m_core->GetSize();
}

void ImageSymbol::InvalidRect(const sm::mat4& mt) const
{
	m_image->InvalidRect(mt);
}

unsigned int ImageSymbol::GetTexID() const
{
	return m_image->GetTexID();
}

void ImageSymbol::LoadResources()
{
	ImageData* img_data = ImageDataMgr::Instance()->GetItem(m_filepath);
	BitmapMgr::Instance()->GetItem(m_filepath, &m_bitmap);
	ImageMgr::Instance()->GetItem(m_filepath, &m_image);
	img_data->Release();

	InitCoreTex();
}

void ImageSymbol::InitCoreTex()
{
	s2::ImageSymbol::Quad q;
	q.xmin = q.ymin = 0;
	q.xmax = m_image->GetClippedWidth();
	q.ymax = m_image->GetClippedHeight();
	const sm::vec2& off = m_image->GetOffset();
	m_core->InitTex(m_image->GetS2Tex(), q, off);
}

}