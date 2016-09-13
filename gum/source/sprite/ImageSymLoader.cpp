#include "ImageSymLoader.h"
#include "ImageSymbol.h"
#include "Image.h"

namespace gum
{

ImageSymLoader::ImageSymLoader(ImageSymbol* sym)
	: m_sym(sym)
{
	if (m_sym) {
		m_sym->AddReference();
	}
}

ImageSymLoader::~ImageSymLoader()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}
}

void ImageSymLoader::Load(const std::string& filepath)
{
	Image* img = ImageMgr::Instance()->Create(filepath);
	if (img) {
		m_sym->SetImage(img);
	}
}

}