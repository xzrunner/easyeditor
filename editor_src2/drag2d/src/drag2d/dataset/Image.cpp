#include "Image.h"

#include "render/GL10.h"
#include "render/Shader.h"
#include "common/config.h"
#include "common/tools.h"
#include "common/Settings.h"

#include "ImageLoader.h"

#include <fstream>
#include <SOIL/SOIL.h>
#include <easyimage.h>

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
	m_filepath = filepath;
 
#ifdef NOT_LOAD_IMAGE
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
		if (Settings::bImageEdgeClip) {
			eimage::ImageProcessor processor(this);
			m_region = processor.trim();
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

	m_region.xMin = -m_width*0.5f;
	m_region.xMax = -m_region.xMin;
	m_region.yMin = -m_height*0.5f;
	m_region.yMax = -m_region.yMin;
#endif
}

void Image::draw(const Rect& r) const
{
	Shader* shader = Shader::Instance();
	shader->sprite();

	float tot_hw = m_width * 0.5f,
		  tot_hh = m_height * 0.5f;
	float txmin = (r.xMin + tot_hw) / m_width,
		txmax = (r.xMax + tot_hw) / m_width,
		tymin = (r.yMin + tot_hh) / m_height,
		tymax = (r.yMax + tot_hh) / m_height;

	GL10::BindTexture(GL10::GL_TEXTURE_2D, m_textureID);
	GL10::Begin(GL10::GL_QUADS);
 		GL10::TexCoord2f(txmin, tymin); GL10::Vertex3f(r.xMin, r.yMin, -1.0f);
 		GL10::TexCoord2f(txmax, tymin); GL10::Vertex3f(r.xMax, r.yMin, -1.0f);
 		GL10::TexCoord2f(txmax, tymax); GL10::Vertex3f(r.xMax, r.yMax, -1.0f);
 		GL10::TexCoord2f(txmin, tymax); GL10::Vertex3f(r.xMin, r.yMax, -1.0f);
	GL10::End();
	GL10::BindTexture(GL10::GL_TEXTURE_2D, NULL);
}

} // d2d