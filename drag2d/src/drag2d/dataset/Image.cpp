#include "Image.h"

#include "render/GL10.h"
#include "render/Shader.h"
#include "render/ShaderNew.h"
#include "common/config.h"
#include "common/tools.h"
#include "common/Settings.h"
#include "common/Exception.h"

#include "ImageLoader.h"

#include <fstream>
#include <SOIL/SOIL.h>
#include <easyimage.h>
#include <wx/filename.h>

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
	if (!wxFileName::FileExists(filepath)) {
		throw Exception("File: %s don't exist!", filepath.ToStdString().c_str());
	}

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
			d2d::Rect r = processor.trim();
			if (r.isValid()) {
				r.translate(d2d::Vector(-m_width*0.5f, -m_height*0.5f));
				m_region = r;
			}
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
  	ShaderNew* shader = ShaderNew::Instance();
  	shader->sprite();
  
  	float tot_hw = m_width * 0.5f,
  		  tot_hh = m_height * 0.5f;
  	float txmin = (r.xMin + tot_hw) / m_width,
  		txmax = (r.xMax + tot_hw) / m_width,
  		tymin = (r.yMin + tot_hh) / m_height,
  		tymax = (r.yMax + tot_hh) / m_height;
  
	float SCALE = 100;

	float vb[16];
	vb[0] = r.xMin / SCALE;
	vb[1] = r.yMin / SCALE;
	vb[2] = txmin;
	vb[3] = tymin;
	vb[4] = r.xMax / SCALE;
	vb[5] = r.yMin / SCALE;
	vb[6] = txmax;
	vb[7] = tymin;
	vb[8] = r.xMax / SCALE;
	vb[9] = r.yMax / SCALE;
	vb[10] = txmax;
	vb[11] = tymax;
	vb[12] = r.xMin / SCALE;
	vb[13] = r.yMax / SCALE;
	vb[14] = txmin;
	vb[15] = tymax;

	shader->Draw(vb, m_textureID);

	shader->Flush();
}

} // d2d