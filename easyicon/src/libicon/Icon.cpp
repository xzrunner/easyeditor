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
		m_img->reload();
	}
}

void Icon::SetImage(d2d::Image* img)
{
	bool changed = m_img != img;
	d2d::obj_assign((d2d::Object*&)m_img, img);
	if (changed) {
		OnImageChanged();
	}
}

}