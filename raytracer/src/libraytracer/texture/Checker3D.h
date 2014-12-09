#ifndef _RT_CHECKER3D_H_
#define _RT_CHECKER3D_H_

#include "Texture.h"

namespace rt
{

class Checker3D : public Texture
{
public:

	virtual RGBColor GetColor(const ShadeRec& sr) const;

	void SetColor1(const RGBColor& c) { m_color1 = c; }
	void SetColor2(const RGBColor& c) { m_color2 = c; }

	void SetSize(float size) { m_size = size; }

private:
	RGBColor m_color1, m_color2;

	float m_size;

}; // Checker3D

}

#endif // _RT_CHECKER3D_H_