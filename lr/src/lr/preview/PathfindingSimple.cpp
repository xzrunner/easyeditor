#include "PathfindingSimple.h"

namespace lr
{
namespace preview
{

PathfindingSimple::PathfindingSimple(const d2d::Rect& region, int row, int col)
	: m_nw(region, row, col)
{
}

void PathfindingSimple::DisableRegion(const d2d::Rect& r, bool disable)
{
	m_nw.SetStatus(r, !disable);
}

void PathfindingSimple::DebugDraw() const
{
	m_nw.DebugDraw();
}

//////////////////////////////////////////////////////////////////////////
// class PathfindingSimple::Network
//////////////////////////////////////////////////////////////////////////

PathfindingSimple::Network::
Network(const d2d::Rect& region, int row, int col)
	: m_region(region)
	, m_row(row)
	, m_col(col)
{
	m_width = m_region.xLength() / m_col;
	m_height = m_region.yLength() / m_row;

	m_nodes = new Node[m_row * m_col];
}

PathfindingSimple::Network::
~Network()
{
	delete[] m_nodes;
}

void PathfindingSimple::Network::
SetStatus(const d2d::Rect& region, bool used)
{
	if (!d2d::Math::isRectContainRect(m_region, region)) {
		return;
	}

	int xs = (region.xMin - m_region.xMin) / m_width,
		xe = (region.xMax - m_region.xMin) / m_width;
	int ys = (region.yMin - m_region.yMin) / m_height,
		ye = (region.yMax - m_region.yMin) / m_height;
	for (int y = ys; y < ye; ++y) {
		for (int x = xs; x < xe; ++x) {
			m_nodes[y * m_col + x].m_used = used;
		}
	}
}

void PathfindingSimple::Network::
DebugDraw() const
{
	int idx = 0;
	for (int y = 0; y < m_row; ++y) {
		for (int x = 0; x < m_col; ++x) {
			if (!m_nodes[idx++].m_used) {
				continue;
			}
			d2d::Vector p0(x * m_width, y * m_height);
			d2d::Vector p1(p0.x + m_width, p0.y + m_height);
			d2d::PrimitiveDraw::rect(d2d::Matrix(), p0, p1, d2d::LIGHT_GREEN_FACE);
		}
	}
}

}
}