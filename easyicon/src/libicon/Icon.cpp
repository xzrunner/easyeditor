#include "Icon.h"

namespace eicon
{

Icon::Icon()
	: m_img(NULL)
{
}

Icon::Icon(d2d::Image* img)
	: m_img(img)
{
	if (m_img) {
		m_img->Retain();
	}
}

Icon::~Icon()
{
	if (m_img) {
		m_img->Release();
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
//	bool changed = m_img != img;
	d2d::obj_assign((d2d::Object*&)m_img, img);
// 	if (changed) {
// 		OnImageChanged();
// 	}
}

void Icon::Draw(const d2d::Matrix& mt, const d2d::Vector r[4]) const
{
	if (!m_img) {
		return;
	}

	d2d::Vector vertices[4];
	float w = m_img->GetClippedWidth(),
		h = m_img->GetClippedHeight();
	for (int i = 0; i < 4; ++i)
	{
		float x = r[i].x * w;
		float y = r[i].y * h;
		vertices[i] = d2d::Math::transVector(d2d::Vector(x, y), mt);
	}

	int texid = m_img->GetTexID();

	d2d::Vector texcoords[4];
	float ori_w = m_img->GetOriginWidth(),
		ori_h = m_img->GetOriginHeight();
	for (int i = 0; i < 4; ++i)
	{
		float x = r[i].x * w / ori_w + 0.5f;
		float y = r[i].y * h / ori_h + 0.5f;
		texcoords[i].set(x, y);
	}

	d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
	shader->sprite();
	shader->Draw(vertices, texcoords, texid);
}

}