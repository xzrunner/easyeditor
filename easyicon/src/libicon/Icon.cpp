#include "Icon.h"

namespace eicon
{

Icon::Icon()
	: m_img(NULL)
{
	m_canvas_region.xMin = m_canvas_region.yMin = 0;
	m_canvas_region.xMax = m_canvas_region.yMax = 1;
}

Icon::Icon(d2d::Image* img)
	: m_img(img)
{
	if (m_img) {
		m_img->Retain();

		d2d::Vector offset = img->GetOffset();
		float ow = img->GetOriginWidth(),
			oh = img->GetOriginHeight();
		float cw = img->GetClippedWidth(),
			ch = img->GetClippedHeight();

		m_canvas_region.xMin = ((ow - cw) * 0.5f + offset.x) / ow;
		m_canvas_region.xMax = (ow - (ow - cw) * 0.5f + offset.x) / ow;
		m_canvas_region.yMin = ((oh - ch) * 0.5f + offset.y) / oh;
		m_canvas_region.yMax = (oh - (oh - ch) * 0.5f + offset.y) / oh;
	}
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
		scr_coords[i] = d2d::Math::transVector(scr_coords[i], mt);
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
		if (x < ret.xMin) ret.xMin = x;
		if (x > ret.xMax) ret.xMax = x;
		if (y < ret.yMin) ret.yMin = y;
		if (y > ret.yMax) ret.yMax = y;
	}

	return ret;
}

void Icon::GetTexCoords(float process, d2d::Vector* tex_coords) const
{
	d2d::Vector bound[4];
	GetBound(process, bound);
	for (int i = 0; i < 4; ++i) {
		tex_coords[i].x = m_canvas_region.xMin + (m_canvas_region.xMax - m_canvas_region.xMin) * bound[i].x;
		tex_coords[i].y = m_canvas_region.yMin + (m_canvas_region.yMax - m_canvas_region.yMin) * bound[i].y;
	}
}

void Icon::GetScreenCoords(float process, const d2d::Vector* tex_coords, 
						   d2d::Vector* screen_coords) const
{
	float w = m_img->GetOriginWidth(),
		h = m_img->GetOriginHeight();	
	float dx = m_canvas_region.xCenter(),
		dy = m_canvas_region.yCenter();
	for (int i = 0; i < 4; ++i) {
		screen_coords[i].x = (tex_coords[i].x - dx) * w;
		screen_coords[i].y = (tex_coords[i].y - dy) * h;
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
}

}