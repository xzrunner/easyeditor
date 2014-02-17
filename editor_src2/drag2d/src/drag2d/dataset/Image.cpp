#include "Image.h"

#include "render/GL10.h"

#include "common/config.h"

#include <SOIL/SOIL.h>
#include <SOIL/stb_image_write.h>

#include "ImageLoader.h"

#include <fstream>

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
	delete m_pixels;
}

bool Image::loadFromFile(const wxString& filepath)
{
	m_filepath = filepath;
 
#ifdef NOT_LOAD_IMAGE
	return true;
#endif

 	reload();
 
 	//  	LibJpeg::ImageData data;
 	//  	LibJpeg::read_JPEG_file(m_filepath.c_str(), data);
 	//  	m_textureID = SOIL_internal_create_OGL_texture(
 	//  		data.pixels, data.width, data.height, data.channels,
 	//  		m_textureID, SOIL_FLAG_INVERT_Y,
 	//  		GL_TEXTURE_2D, GL_TEXTURE_2D,
 	//  		GL_MAX_TEXTURE_SIZE );
 	//  	delete[] data.pixels;
 
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

 		removeTransparentBorder();

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
// 	std::ifstream fin(m_filepath.fn_str(), std::ios::binary);
// 	assert(!fin.fail());
// 
// 	// get length of file:
// 	fin.seekg (0, fin.end);
// 	int length = fin.tellg();
// 	fin.seekg (0, fin.beg);
// 
// 	char* buffer = new char[length];
// 	fin.read (buffer,length);
// 	m_pixels = ImageLoader::loadTexture(buffer, m_width, m_height, m_textureID, m_format);

	m_pixels = ImageLoader::loadTexture(m_filepath.ToStdString(), m_width, m_height, m_textureID, m_format);

	m_region.xMin = -m_width*0.5f;
	m_region.xMax = -m_region.xMin;
	m_region.yMin = -m_height*0.5f;
	m_region.yMax = -m_region.yMin;

// 	delete[] buffer;
#endif
}

void Image::draw(const Rect& r) const
{
// 	GL10::Enable(GL10::GL_BLEND);
// 	GL10::BlendFunc(GL10::GL_SRC_ALPHA, GL10::GL_ONE_MINUS_SRC_ALPHA);

// 	GL10::Color4f(1.0f, 1.0f, 1.0f, 1.0f);

	float hWidth = r.xLength() * 0.5f,
		hHeight = r.yLength() * 0.5f;
	float txmin = r.xMin / m_width ,
		txmax = r.xMax / m_width ,
		tymin = r.yMin / m_height ,
		tymax = r.yMax / m_height ;

	GL10::BindTexture(GL10::GL_TEXTURE_2D, m_textureID);
	GL10::Begin(GL10::GL_QUADS);
		GL10::TexCoord2f(txmin, tymin); GL10::Vertex3f(-hWidth, -hHeight, -1.0f);
		GL10::TexCoord2f(txmax, tymin); GL10::Vertex3f( hWidth, -hHeight, -1.0f);
		GL10::TexCoord2f(txmax, tymax); GL10::Vertex3f( hWidth,  hHeight, -1.0f);
		GL10::TexCoord2f(txmin, tymax); GL10::Vertex3f(-hWidth,  hHeight, -1.0f);
	GL10::End();
	GL10::BindTexture(GL10::GL_TEXTURE_2D, NULL);

	//GL10::Disable(GL10::GL_BLEND);
}

const unsigned char* Image::clip(int xmin, int xmax, int ymin, int ymax)
{
	int channels;
	int size = 0;
	switch(m_format)
	{
	case 0x1907:	// GL_RGB
		channels = 3;
		return NULL;
	case 0x1908:	// GL_RGBA
		channels = 4;
		break;
	default:
		return NULL;
	}

	if (xmin < 0 || xmin >= m_width ||
		xmax < 0 || xmax >= m_width ||
		ymin < 0 || ymin >= m_height ||
		ymax < 0 || ymax >= m_height ||
		xmin >= xmax || ymin >= ymax)
		return NULL;

	int w = xmax - xmin,
		h = ymax - ymin;
	unsigned char* pixels = new unsigned char[w * h * channels];
	int line_size = channels * w;
	for (int i = 0; i < h; ++i)
	{
		int from = (m_width * (ymin + i) + xmin) * channels,
			to = i * w * channels;
		memcpy(&pixels[to], &m_pixels[from], line_size);
	}
	return pixels;
}

void Image::writeToFile(const unsigned char* pixels, int width, int height, 
						const std::string& filename)
{
	assert(m_format == 0x1908);
	stbi_write_png(filename.c_str(), width, height, 4, pixels, 0);
}

void Image::removeTransparentBorder()
{
	int channels;
	int numBytes = 0;
	switch(m_format)
	{
	case GL10::GL_RGB:
		return;
	case GL10::GL_RGBA:
		channels = 4;
		numBytes = m_width * m_height * channels;
		break;
	default:
		return;
	}

	if(numBytes)
	{		
		// down
		m_region.yMin = 0;
		for (size_t i = 0; i < m_height; ++i)
		{
			size_t j = 0;
			for ( ; j < m_width; ++j)
				if (!isTransparent(m_pixels, j, i, channels))
					break;
			if (j == m_width) ++m_region.yMin;
			else break;
		}
		// up
 		m_region.yMax = m_height;
 		for (int i = m_height - 1; i >= 0; --i)
		{
			size_t j = 0;
			for ( ; j < m_width; ++j)
				if (!isTransparent(m_pixels, j, i, channels))
					break;
			if (j == m_width) --m_region.yMax;
			else break;
		}
		// left
		m_region.xMin = 0;
		for (size_t i = 0; i < m_width; ++i)
		{
			size_t j = 0;
			for ( ; j < m_height; ++j)
				if (!isTransparent(m_pixels, i, j, channels))
					break;
			if (j == m_height) ++m_region.xMin;
			else break;
		}
		// right
		m_region.xMax = m_width;
		for (int i = m_width - 1; i >= 0; --i)
		{
			size_t j = 0;
			for ( ; j < m_height; ++j)
				if (!isTransparent(m_pixels, i, j, channels))
					break;
			if (j == m_height) --m_region.xMax;
			else break;
		}

//		m_region.translate(Vector(-m_width*0.5f, -m_height*0.5f));
	}
}

bool Image::isTransparent(unsigned char* pixels, int x, int y, int channels)
{
// 	int ptr = (m_width * y + x) * channels;
// 	for (size_t i = 0; i < channels; ++i)
// 		if (pixels[ptr+i])
// 			return false;
// 	return true;

	return pixels[(m_width * y + x) * channels + channels - 1] == 0;
}
} // d2d