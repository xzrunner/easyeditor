#include "ImageTrim.h"
#include "ImageTrimRaw.h"

namespace eimage
{

ImageTrim::ImageTrim(const d2d::Image* image)
	: m_image(image)
{
}

d2d::Rect ImageTrim::Trim() const
{
	ImageTrimRaw trim(m_image->GetPixelData(), m_image->GetOriginWidth(),
		m_image->GetOriginHeight());
	return trim.Trim();
}

}