#ifndef _EASYIMAGE_OUTLINE_TO_TRI_STRIP_H_
#define _EASYIMAGE_OUTLINE_TO_TRI_STRIP_H_

#include <ee/Image.h>

#include <cu/cu_stl.h>

namespace eimage
{

class OutlineToTriStrip
{
public:
	OutlineToTriStrip(const CU_VEC<sm::vec2>& outline);

	void TransToStrip();

	const CU_VEC<CU_VEC<sm::vec2> >& GetStrip() const {
		return m_strips;
	}

private:
	CU_VEC<sm::vec2> m_outline;

	CU_VEC<CU_VEC<sm::vec2> > m_strips;

}; // OutlineToTriStrip

}

#endif // _EASYIMAGE_OUTLINE_TO_TRI_STRIP_H_