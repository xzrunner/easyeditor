#include "ImageSymbol.h"
#include "Image.h"

#include <dtex_facade.h>

namespace gum
{

ImageSymbol::ImageSymbol(const std::string& filepath)
{
	m_img = ImageMgr::Instance()->Create(filepath);
	if (!m_img) {
		return;
	}

	s2::ImageSymbol::Quad q;
	q.xmin = q.ymin = 0;
	sm::ivec2 sz = m_img->GetSize();
	q.xmax = sz.x;
	q.ymax = sz.y;

	InitTex(m_img->GetS2Tex(), q, sm::vec2(0, 0));
}

ImageSymbol::~ImageSymbol()
{
	if (m_img) {
		m_img->RemoveReference();
	}
}

void ImageSymbol::QueryTexcoords(float* texcoords, int& texid) const
{
	texid = m_img->GetTexID();
	float txmin, txmax, tymin, tymax;
	txmin = tymin = 0;
	txmax = tymax = 1;
	texcoords[0] = txmin; texcoords[1] = tymin;
	texcoords[2] = txmin; texcoords[3] = tymax;
	texcoords[4] = txmax; texcoords[5] = tymax;
	texcoords[6] = txmax; texcoords[7] = tymin;
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