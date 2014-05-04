#include "PolylineFileAdapter.h"

#include <fstream>

#include "dataset/ChainShape.h"

namespace d2d
{

PolylineFileAdapter::PolylineFileAdapter(std::vector<ChainShape*>& chains)
	: m_chains(chains)
{
}

} // d2d