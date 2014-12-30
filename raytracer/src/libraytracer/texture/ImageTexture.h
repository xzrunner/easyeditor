#ifndef _RT_IMAGE_TEXTURE_H_
#define _RT_IMAGE_TEXTURE_H_

#include "Texture.h"

namespace rt
{

class Image;
class Mapping;

class ImageTexture : public Texture
{
public:
	ImageTexture();
	ImageTexture(Image*	image);
	virtual ~ImageTexture();

	virtual RGBColor GetColor(const ShadeRec& sr) const;

	void SetImage(Image* image);
	void SetMapping(Mapping* mapping);

private:
	int 		m_hres;			// horizontal resolution of the image
	int			m_vres;			// vertical resolution of the image
	Image*		m_image;		// the image
	Mapping*	m_mapping;		// mapping technique used, if any

}; // ImageTexture

}

#endif // _RT_IMAGE_TEXTURE_H_