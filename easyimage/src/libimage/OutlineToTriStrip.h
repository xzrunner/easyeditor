#ifndef _EASYIMAGE_OUTLINE_TO_TRI_STRIP_H_
#define _EASYIMAGE_OUTLINE_TO_TRI_STRIP_H_

#include <drag2d.h>

namespace eimage
{

class OutlineToTriStrip
{
public:
	OutlineToTriStrip(const std::vector<d2d::Vector>& outline);

	void TransToStrip();

	const std::vector<std::vector<d2d::Vector> >& GetStrip() const {
		return m_strips;
	}

private:
	std::vector<d2d::Vector> m_outline;

	std::vector<std::vector<d2d::Vector> > m_strips;

}; // OutlineToTriStrip

}

#endif // _EASYIMAGE_OUTLINE_TO_TRI_STRIP_H_