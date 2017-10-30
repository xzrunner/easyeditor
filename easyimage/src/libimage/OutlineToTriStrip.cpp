#include "OutlineToTriStrip.h"

#include <ee/SGI.h>

#include <SM_Triangulation.h>

namespace eimage
{

OutlineToTriStrip::OutlineToTriStrip(const CU_VEC<sm::vec2>& outline)
	: m_outline(outline)
{
}

void OutlineToTriStrip::TransToStrip()
{
	CU_VEC<sm::vec2> tris;
	triangulate_normal(m_outline, tris);
	ee::SGI::Do(tris, m_strips);
}

}