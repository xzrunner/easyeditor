#include "Snapshoot.h"
#include "FBO.h"
#include "Symbol.h"
#include "ImageSaver.h"

namespace ee
{

static const int DEFAULT_WIDTH = 2048;
static const int DEFAULT_HEIGHT = 2048;

Snapshoot::Snapshoot()
	: m_width(DEFAULT_WIDTH)
	, m_height(DEFAULT_HEIGHT)
{
	m_fbo = new FBO(DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

Snapshoot::Snapshoot(int width, int height)
	: m_width(width)
	, m_height(height)
{
	m_fbo = new FBO(width, height);
}

Snapshoot::~Snapshoot()
{
	m_fbo->RemoveReference();	
}

unsigned char* Snapshoot::OutputToMemory(const Symbol* symbol, bool whitebg,
										 float scale) const
{
	m_fbo->DrawSymbol(symbol, whitebg, scale);

	sm::vec2 sz = symbol->GetBounding().Size();
	sz *= scale;

	int w = static_cast<int>(sz.x),
		h = static_cast<int>(sz.y);
	if (w > m_fbo->GetWidth() || h > m_fbo->GetHeight()) {
		return NULL;
	}

	size_t size = w * h * 4;
	unsigned char* pixels = new unsigned char[size];
	if(!pixels) {
		return NULL;
	}
	memset(&pixels[0], 0, size);	

	m_fbo->ReadPixels(pixels, w, h);

	return pixels;
}

void Snapshoot::OutputToImageFile(const Symbol* symbol, const std::string& filename,
								  float scale) const
{
	sm::vec2 sz = symbol->GetBounding().Size() * scale;
	int w = static_cast<int>(sz.x),
		h = static_cast<int>(sz.y);
	unsigned char* pixels = OutputToMemory(symbol, false, scale);
	if (pixels) {
		ImageSaver::StoreToFile(pixels, w, h, 4, filename, ImageSaver::e_png, false);
		delete[] pixels;
	}
}

void Snapshoot::DrawSprite(const Sprite* sprite, bool clear,
						   float dx, float dy) const
{
	m_fbo->DrawSprite(sprite, clear, m_width, m_height, dx, dy);
}

void Snapshoot::DrawSprite(const Sprite* sprite, bool clear, int width, int height,
						   float dx, float dy, float scale) const
{
	m_fbo->DrawSprite(sprite, clear, width, height, dx, dy, scale);
}

void Snapshoot::DrawShape(const Shape* shape, bool clear) const
{
	m_fbo->DrawShape(shape, clear, m_width, m_height);
}

void Snapshoot::SaveToFile(const std::string& filename) const
{
	SaveToFile(filename, m_width, m_height);
}

void Snapshoot::SaveToFile(const std::string& filename, int width, int height) const
{
	size_t size = width * height * 4;
	unsigned char* pixels = new unsigned char[size];
	if(!pixels) return;
	memset(&pixels[0], 0, size);	

	m_fbo->ReadPixels(pixels, width, height);

	ImageSaver::StoreToFile(pixels, width, height, 4, filename, ImageSaver::e_png, false);

//	ImageIO::Write(pixels, m_width, m_height, filename);

	delete[] pixels;
}

}