#include "EditNodesAtomicOP.h"
#include "EditedPolyShape.h"

namespace eshape
{
namespace edit_nodes
{

ModifyNodesAOP::ModifyNodesAOP(const std::vector<EditedPolyShape*>& polylines, 
							   const std::vector<std::vector<sm::vec2> >& dst_polylines)
{
	m_polylines.reserve(polylines.size());
	for (size_t i = 0, n = polylines.size(); i < n; ++i)
	{
		polylines[i]->AddReference();
		m_polylines.push_back(polylines[i]);

		m_src.push_back(polylines[i]->GetVertices());
		m_dst.push_back(dst_polylines[i]);
	}
}

void ModifyNodesAOP::Undo()
{
	for (size_t i = 0, n = m_polylines.size(); i< n; ++i)
		m_polylines[i]->SetVertices(m_src[i]);
}

void ModifyNodesAOP::Redo()
{
	for (size_t i = 0, n = m_polylines.size(); i< n; ++i)
		m_polylines[i]->SetVertices(m_dst[i]);
}

} // edit_nodes
} // libshape