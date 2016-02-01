#include "EditNodesAtomicOP.h"
#include "ChainShape.h"

namespace eshape
{
namespace edit_nodes
{

ModifyNodesAOP::ModifyNodesAOP(const std::vector<ChainShape*>& chains, 
							   const std::vector<std::vector<ee::Vector> >& dstChains)
{
	m_chains.reserve(chains.size());
	for (size_t i = 0, n = chains.size(); i < n; ++i)
	{
		chains[i]->Retain();
		m_chains.push_back(chains[i]);

		m_src.push_back(chains[i]->GetVertices());
		m_dst.push_back(dstChains[i]);
	}
}

void ModifyNodesAOP::Undo()
{
	for (size_t i = 0, n = m_chains.size(); i< n; ++i)
		m_chains[i]->Load(m_src[i]);
}

void ModifyNodesAOP::Redo()
{
	for (size_t i = 0, n = m_chains.size(); i< n; ++i)
		m_chains[i]->Load(m_dst[i]);
}

} // edit_nodes
} // libshape