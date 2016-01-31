#include "OutlineToTriStrip.h"

namespace eimage
{

OutlineToTriStrip::OutlineToTriStrip(const std::vector<ee::Vector>& outline)
	: m_outline(outline)
{
}

void OutlineToTriStrip::TransToStrip()
{
	std::vector<ee::Vector> tris;
	ee::Triangulation::Normal(m_outline, tris);
	ee::Triangulation::Strips(tris, m_strips);
}

}