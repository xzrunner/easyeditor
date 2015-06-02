#include "Shadow.h"

namespace eshadow
{

Shadow::Shadow()
	: m_inner_loop(NULL)
	, m_outer_loop(NULL)
{
}

Shadow::~Shadow()
{
	if (m_inner_loop) {
		m_inner_loop->Release();
	}
	if (m_outer_loop) {
		m_outer_loop->Release();
	}
}

void Shadow::Draw(const d2d::Matrix& mt) const
{
	d2d::PrimitiveDraw::DrawTriangles(mt, m_tris, m_colors);
}

void Shadow::Build()
{
	
}

}