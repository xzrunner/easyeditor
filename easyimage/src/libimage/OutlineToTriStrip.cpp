#include "OutlineToTriStrip.h"

namespace eimage
{

OutlineToTriStrip::OutlineToTriStrip(const std::vector<d2d::Vector>& outline)
	: m_outline(outline)
{
}

void OutlineToTriStrip::TransToStrip()
{
	std::vector<d2d::Vector> tris;
	d2d::Triangulation::normal(m_outline, tris);
	d2d::Triangulation::strips(tris, m_strips);
}

}