#include "Icon.h"

namespace eicon
{

Icon::Icon()
	: m_img(NULL)
{
}

Icon::Icon(d2d::Image* img)
	: m_img(NULL)
{
	SetImage(img);
}

Icon::~Icon()
{
	if (m_img) {
		m_img->Release();
	}
}

void Icon::Draw(const d2d::Matrix& mt, float process) const
{
	if (!m_img) {
		return;
	}

	// texid
	int texid = m_img->GetTexID();

	// tex_coords
	d2d::Vector tex_coords[4];
	GetTexCoords(process, tex_coords);

	// vertices
	d2d::Vector scr_coords[4];
	GetScreenCoords(process, tex_coords, scr_coords);
	for (int i = 0; i < 4; ++i) {
		scr_coords[i] = d2d::Math2D::TransVector(scr_coords[i], mt);
	}

	d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
	shader->sprite();
	shader->Draw(scr_coords, tex_coords, texid);
}

d2d::Rect Icon::GetRegion(float process) const
{
	d2d::Rect ret;

	if (!m_img) {
		return ret;
	}
	
	d2d::Vector tex_coords[4];
	GetTexCoords(process, tex_coords);

	d2d::Vector scr_coords[4];
	GetScreenCoords(process, tex_coords, scr_coords);
	for (int i = 0; i < 4; ++i) {
		float x = scr_coords[i].x,
			y = scr_coords[i].y;
		if (x < ret.xmin) ret.xmin = x;
		if (x > ret.xmax) ret.xmax = x;
		if (y < ret.ymin) ret.ymin = y;
		if (y > ret.ymax) ret.ymax = y;
	}

	return ret;
}

void Icon::GetTexCoords(float process, d2d::Vector* tex_coords) const
{
	d2d::Vector bound[4];
	GetBound(process, bound);
	for (int i = 0; i < 4; ++i) {
		tex_coords[i].x = bound[i].x;
		tex_coords[i].y = bound[i].y;
	}
}

void Icon::GetScreenCoords(float process, const d2d::Vector* tex_coords, 
						   d2d::Vector* screen_coords) const
{
	float w = m_img->GetClippedWidth(),
		h = m_img->GetClippedHeight();	
	for (int i = 0; i < 4; ++i) {
		screen_coords[i].x = (tex_coords[i].x - 0.5f) * w;
		screen_coords[i].y = (tex_coords[i].y - 0.5f) * h;
	}
}

void Icon::ReloadTexture() const
{
	if (m_img) {
		m_img->ReloadTexture();
	}
}

void Icon::SetImage(d2d::Image* img)
{
	d2d::obj_assign<d2d::Image>(m_img, img);
	AfterSetImage();
}

}