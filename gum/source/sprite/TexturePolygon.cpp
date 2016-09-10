#include "TexturePolygon.h"
#include "Image.h"

namespace gum
{

TexturePolygon::TexturePolygon(const std::string& filepath)
{
	m_img = ImageMgr::Instance()->Create(filepath);
	if (!m_img) {
		return;
	}

	s2::Texture* tex = m_img->GetS2Tex();
	cu::RefCountObjAssign(m_tex, tex);
}

TexturePolygon::~TexturePolygon()
{
	if (m_img) {
		m_img->RemoveReference();
	}
}

}