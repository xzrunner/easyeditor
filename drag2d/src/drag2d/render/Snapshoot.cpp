#include "Snapshoot.h"
#include "FBO.h"

#include "dataset/ISymbol.h"
#include "dataset/ImageSaver.h"

namespace d2d
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
	m_fbo->Release();	
}

unsigned char* Snapshoot::OutputToMemory(const ISymbol* symbol, bool whitebg,
										 float scale) const
{
	m_fbo->DrawSymbol(symbol, whitebg, scale);

	int w = symbol->GetSize().xLength() * scale,
		h = symbol->GetSize().yLength() * scale;
	size_t size = w*h*4;
	unsigned char* pixels = new unsigned char[size];
	if(!pixels) return NULL;
	memset(&pixels[0], 0, size);	

	m_fbo->ReadPixels(pixels, w, h);

	return pixels;
}

void Snapshoot::OutputToImageFile(const ISymbol* symbol, const std::string& filename,
								  float scale) const
{
	int w = symbol->GetSize().xLength() * scale,
		h = symbol->GetSize().yLength() * scale;
	unsigned char* pixels = OutputToMemory(symbol, false, scale);
	ImageSaver::StoreToFile(pixels, w, h, 4, filename, ImageSaver::e_png);
	delete[] pixels;
}

void Snapshoot::DrawSprite(const ISprite* sprite, bool clear,
						   float dx, float dy) const
{
	m_fbo->DrawSprite(sprite, clear, m_width, m_height, dx, dy);
}

void Snapshoot::DrawSprite(const ISprite* sprite, bool clear, int width, int height,
						   float dx, float dy) const
{
	m_fbo->DrawSprite(sprite, clear, width, height, dx, dy);
}

void Snapshoot::DrawShape(const IShape* shape, bool clear) const
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

	ImageSaver::StoreToFile(pixels, width, height, 4, filename, ImageSaver::e_png);

//	ImageIO::Write(pixels, m_width, m_height, filename);

	delete[] pixels;
}

}