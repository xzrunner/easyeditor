#include "ImageTexture.h"

#include "mapping/Mapping.h"
#include "texture/Image.h"
#include "utilities/ShadeRec.h"

namespace rt
{

ImageTexture::ImageTexture()
	: m_image(NULL)
	, m_mapping(NULL)
{
}

ImageTexture::ImageTexture(Image* image)
	: m_hres(image->GetHres())
	, m_vres(image->GetVres())
	, m_mapping(NULL)
{
	if (image) {
		image->Retain();
		m_image = image;
	}
}

ImageTexture::~ImageTexture()
{
	if (m_image) {
		m_image->Release();
	}
	if (m_mapping) {
		m_mapping->Release();
	}
}

RGBColor ImageTexture::GetColor(const ShadeRec& sr) const
{
	int row, column;
	if (m_mapping) {
		m_mapping->GetTexelCoordinates(sr.local_hit_point, m_hres, m_vres, row, column);
	} else {
		row 	= (int)(sr.v * (m_vres - 1));
		column 	= (int)(sr.u * (m_hres - 1));
	}

	return m_image->GetColor(row, column);
}

void ImageTexture::SetImage(Image* image)
{
	obj_assign((const Object*&)m_image, image);
}

void ImageTexture::SetMapping(Mapping* mapping)
{
	obj_assign((const Object*&)m_mapping, mapping);
}

}