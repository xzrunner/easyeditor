#include "Shadow.h"

namespace eshadow
{

Shadow::Shadow(float radius)
	: m_radius(radius)
{
}

Shadow::~Shadow()
{
}

void Shadow::Draw(const d2d::Matrix& mt) const
{
	d2d::PrimitiveDraw::DrawTriangles(mt, m_tris, m_colors);
}

void Shadow::Build()
{
	if (m_inner_loop.empty()) {
		return;
	}

	BuildOuterLoop();
	assert(m_inner_loop.size() == m_outer_loop.size());

	
}

void Shadow::BuildOuterLoop()
{
	m_outer_loop.clear();

	int sz = m_inner_loop.size();
	m_outer_loop.reserve(sz);
	for (int i = 0; i < sz; ++i)
	{
		int curr = i,
			curr_prev = (curr - 1 + sz) % sz;
		int next = (i + 1) % sz,
			next_next = (next + 1) % sz,
	}
}

}