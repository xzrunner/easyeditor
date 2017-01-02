#include "ImageSymbol.h"
#include "Bitmap.h"
#include "Image.h"
#include "Sprite.h"
#include "ImageData.h"

#include <unirender/Texture.h>
#include <unirender/RenderTarget.h>
#include <sprite2/ImageSymbol.h>
#include <sprite2/RenderParams.h>
#include <sprite2/S2_RVG.h>
#include <gum/RenderContext.h>
#include <gum/RenderTarget.h>

namespace ee
{

ImageSymbol::ImageSymbol()
	: m_image(NULL)
{
}

ImageSymbol::ImageSymbol(Image* image, const std::string& filename)
	: m_image(image)
{
	m_image->AddReference();
	InitCoreTex();

	m_filepath = filename;
}

ImageSymbol::~ImageSymbol()
{
	if (m_bitmap) {
		m_bitmap->RemoveReference();
	}
	if (m_image) {
		m_image->RemoveReference();
	}
}

void ImageSymbol::ReloadTexture() const
{
	m_image->ReloadTexture();
}

void ImageSymbol::InvalidRect(const sm::mat4& mt) const
{
	m_image->InvalidRect(mt);
}

unsigned int ImageSymbol::GetTexID() const
{
	return m_image->GetTexID();
}

void ImageSymbol::QueryTexcoords(float* texcoords, int& texid) const
{
	GetImage()->QueryTexcoords(texcoords, &texid);
}

void ImageSymbol::Proj2Screen(float px, float py, int w, int h, float& sx, float& sy) const
{
	// todo
// 	const Camera* cam = CameraMgr::Instance()->GetCamera();
// 	assert(cam);
// 	sm::vec2 v = cam->TransPosProjectToScreen(sm::vec2(px, py), w, h);
// 	sx = v.x;
// 	sy = v.y;
}

bool ImageSymbol::IsOrthoCam() const
{
	// todo
	return true;
// 	const Camera* cam = CameraMgr::Instance()->GetCamera();
// 	assert(cam);
// 	return cam->Type() == "ortho";
}

void ImageSymbol::GetScreenSize(int& w, int& h) const
{
	gum::RenderContext* rc = gum::RenderContext::Instance();
	w = rc->GetWidth();
	h = rc->GetHeight();
}

float ImageSymbol::GetP3dCamAngle() const
{
// 	const Camera* cam = CameraMgr::Instance()->GetCamera();
// 	const Pseudo3DCamera* pcam = static_cast<const Pseudo3DCamera*>(cam);
// 	return pcam->GetAngle();

	return 0;
}

int ImageSymbol::GetScreenCacheTexid() const
{
	return gum::RenderTarget::Instance()->GetScreen0()->GetTexture()->ID();
}

void ImageSymbol::LoadResources()
{
	ImageData* img_data = ImageDataMgr::Instance()->GetItem(m_filepath);
	BitmapMgr::Instance()->GetItem(m_filepath, &m_bitmap);
	ImageMgr::Instance()->GetItem(m_filepath, &m_image);
	img_data->RemoveReference();

	InitCoreTex();
}

void ImageSymbol::InitCoreTex()
{
	s2::ImageSymbol::Quad q;
	q.xmin = q.ymin = 0;
	q.xmax = m_image->GetClippedWidth();
	q.ymax = m_image->GetClippedHeight();
	const sm::vec2& off = m_image->GetOffset();
	InitTex(m_image->GetS2Tex(), q, off);
}

}