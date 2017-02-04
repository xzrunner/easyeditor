#include "ImageSymbol.h"
#include "Bitmap.h"
#include "Image.h"
#include "Sprite.h"
#include "ImageData.h"

#include <unirender/Texture.h>
#include <unirender/RenderTarget.h>
#include <gimg_typedef.h>
#include <gimg_import.h>
#include <sprite2/ImageSymbol.h>
#include <sprite2/RenderParams.h>
#include <sprite2/S2_RVG.h>
#include <gum/RenderContext.h>
#include <gum/RenderTarget.h>
#include <gum/AsyncTask.h>
#include <gum/FilepathHelper.h>
#include <gum/Config.h>

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

void ImageSymbol::InvalidRect(const sm::mat4& mt) const
{
	m_image->InvalidRect(mt);
}

bool ImageSymbol::QueryTexcoords(float* texcoords, int& texid) const
{
	Image* img = GetImage();
	if (!img) {
		return false;
	}

	img->QueryTexcoords(texcoords, &texid);
	return true;
}

unsigned int ImageSymbol::GetTexID() const
{
	return m_image->GetTexID();
}

void ImageSymbol::SetImage(Image* img)
{
	cu::RefCountObjAssign(m_image, img);
}

bool ImageSymbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath)) {
		return false;
	}
	LoadSync();
	return true;
}

void ImageSymbol::InitCoreTex()
{
	sm::i16_rect r;
	r.xmin = r.ymin = 0;
	r.xmax = m_image->GetClippedWidth();
	r.ymax = m_image->GetClippedHeight();

	const sm::vec2& offset = m_image->GetOffset();

	InitTex(m_image->GetS2Tex(), r, offset);
}

void ImageSymbol::LoadSync()
{
	m_bitmap = new Bitmap(m_filepath);
	ImageMgr::Instance()->GetItem(m_filepath, &m_image);
	InitCoreTex();
}

void ImageSymbol::LoadAsync()
{
	gum::AsyncTask::Instance()->Load(m_filepath, LoadCB, ParserCB, this);

	int w, h;
	gimg_read_header(m_filepath.c_str(), &w, &h);
	m_size.Build(w, h);
}

void ImageSymbol::LoadCB(const char* filepath, void (*unpack)(const void* data, size_t size, void* ud), void* ud)
{
	int w, h, fmt;
	uint8_t* pixels = gimg_import(filepath, &w, &h, &fmt);
	if (fmt == GPF_RGBA && gum::Config::Instance()->GetPreMulAlpha()) {
		gimg_pre_mul_alpha(pixels, w, h);
	}

	assert(fmt == GPF_RGB || fmt == GPF_RGBA);
	int c = fmt == GPF_RGB ? 3 : 4;
	int pixel_sz = w * h * c;

	int data_sz = pixel_sz + sizeof(int) * 3;
	uint8_t* data = (uint8_t*)malloc(data_sz);
	uint8_t* ptr = data;

	memcpy(ptr, &w, sizeof(w));
	ptr += sizeof(w);
	memcpy(ptr, &h, sizeof(w));
	ptr += sizeof(h);
	memcpy(ptr, &fmt, sizeof(w));
	ptr += sizeof(fmt);
	memcpy(ptr, pixels, pixel_sz);	

	unpack(data, data_sz, ud);
}

void ImageSymbol::ParserCB(const void* data, size_t size, void* ud)
{
	const uint8_t* ptr = (const uint8_t*)data;

	int w, h, fmt;
	memcpy(&w, ptr, sizeof(w));
	ptr += sizeof(w);
	memcpy(&h, ptr, sizeof(h));
	ptr += sizeof(h);
	memcpy(&fmt, ptr, sizeof(fmt));
	ptr += sizeof(fmt);

	ImageSymbol* sym = static_cast<ImageSymbol*>(ud);
	Bitmap* bmp = new Bitmap(sym->GetFilepath(), const_cast<uint8_t*>(ptr), w, h, fmt);
	sym->SetBitmap(bmp);

	assert(fmt == GPF_RGB || fmt == GPF_RGBA);
	sym->SetImage(new Image(ptr, w, h, fmt));
	sym->InitCoreTex();
}

}