#include "ImageSymbol.h"
#include "Image.h"

#include <dtex_facade.h>

namespace gum
{
 
ImageSymbol::ImageSymbol()
	: m_img(NULL)
{
	m_texcoords[0] = 0; m_texcoords[1] = 0;
	m_texcoords[2] = 0; m_texcoords[3] = 1;
	m_texcoords[4] = 1; m_texcoords[5] = 1;
	m_texcoords[6] = 1; m_texcoords[7] = 0;
}

ImageSymbol::~ImageSymbol()
{
	if (m_img) {
		m_img->RemoveReference();
	}
}

void ImageSymbol::SetImage(Image* img)
{
	cu::RefCountObjAssign(m_img, img);

	s2::ImageSymbol::Quad q;
	q.xmin = q.ymin = 0;
	sm::ivec2 sz = m_img->GetSize();
	q.xmax = sz.x;
	q.ymax = sz.y;

	InitTex(m_img->GetS2Tex(), q, sm::vec2(0, 0));	
}

void ImageSymbol::SetRegion(const sm::ivec2& min, const sm::ivec2& max)
{
	bool rotate = false;
	if (max.y < min.y) {
		rotate = true;
	}

	float hw, hh;
	sm::ivec2 sz = m_img->GetSize();
	float txmin, tymin, txmax, tymax;
	txmin = (float)min.x / sz.x;
	tymin = (float)min.y / sz.y;
	txmax = (float)max.x / sz.x;
	tymax = (float)max.y / sz.y;
	if (rotate) 
	{
		hw = (min.y - max.y) * 0.5f;
		hh = (max.x - min.x) * 0.5f;

		m_texcoords[0] = txmin; m_texcoords[1] = tymin;
		m_texcoords[2] = txmax; m_texcoords[3] = tymin;
		m_texcoords[4] = txmax; m_texcoords[5] = tymax;
		m_texcoords[6] = txmin; m_texcoords[7] = tymax;
	}
	else
	{
		hw = (max.x - min.x) * 0.5f;
		hh = (max.y - min.y) * 0.5f;

		m_texcoords[0] = txmin; m_texcoords[1] = tymin;
		m_texcoords[2] = txmin; m_texcoords[3] = tymax;
		m_texcoords[4] = txmax; m_texcoords[5] = tymax;
		m_texcoords[6] = txmax; m_texcoords[7] = tymin;
	}

	m_quad.xmin = min.x;
	m_quad.ymin = min.y;
	m_quad.xmax = max.x;
	m_quad.ymax = max.y;

	m_size.xmin = -hw;
	m_size.ymin = -hh;
	m_size.xmax = hw;
	m_size.ymax = hh;
}

void ImageSymbol::QueryTexcoords(float* texcoords, int& texid) const
{
	texid = m_img->GetTexID();
	memcpy(texcoords, m_texcoords, sizeof(m_texcoords));
}

void ImageSymbol::Proj2Screen(float px, float py, int w, int h, float& sx, float& sy) const
{
	sx = px;
	sy = py;
}

bool ImageSymbol::IsOrthoCam() const
{
	return true;
}

void ImageSymbol::GetScreenSize(int& w, int& h) const
{
	float _w, _h, s;
	dtex_get_screen(&_w, &_h, &s);
	w = _w;
	h = _h;
}

float ImageSymbol::GetP3dCamAngle() const
{
	return 0;
}

int ImageSymbol::GetScreenCacheTexid() const
{
	return dtexf_cs1_get_texture_id();
}

}