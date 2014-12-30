#ifndef _RT_IMAGE_H_
#define _RT_IMAGE_H_

#include "utilities/RGBColor.h"
#include "utilities/Object.h"

#include <vector>

namespace rt
{

class Image : public Object
{
public:
	Image();

	void ReadPPMFile(const char* file_name);

	const RGBColor& GetColor(int x, int y) const;

	int GetHres() const { return m_hres; }
	int GetVres() const { return m_vres; }

private:
	int m_hres, m_vres;

	std::vector<RGBColor> m_pixels;

}; // Image

}

#endif // _RT_IMAGE_H_