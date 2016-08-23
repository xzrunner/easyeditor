#include "Icon.h"

#include <ee/Image.h>

#include <sprite2/Texture.h>

namespace eicon
{

Icon::Icon()
	: m_img(NULL)
{
}

Icon::Icon(ee::Image* img)
	: m_img(NULL)
{
	SetImage(img);
}

Icon::~Icon()
{
	if (m_img) {
		m_img->RemoveReference();
	}
}

void Icon::SetImage(ee::Image* img)
{
	cu::RefCountObjAssign(m_img, img);
	if (m_tex) {
		delete m_tex;
	}
	m_tex = m_img->GetS2Tex();
	Update();
}

}