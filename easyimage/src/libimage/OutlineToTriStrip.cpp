#include "OutlineToTriStrip.h"

#include <ee/Triangulation.h>

namespace eimage
{

OutlineToTriStrip::OutlineToTriStrip(const std::vector<sm::vec2>& outline)
	: m_outline(outline)
{
}

void OutlineToTriStrip::TransToStrip()
{
	std::vector<sm::vec2> tris;
	ee::Triangulation::Normal(m_outline, tris);
	ee::Triangulation::Strips(tris, m_strips);
}

}