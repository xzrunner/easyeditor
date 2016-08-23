#include "Symbol.h"
#include "Mesh.h"
#include "FileIO.h"
#include "MeshFactory.h"

namespace emesh
{

Symbol::Symbol()
	: m_pause(false)
{
}

Symbol::Symbol(ee::Symbol* base)
{
	m_mesh = MeshFactory::Instance()->CreateMesh(base);
}

void Symbol::LoadResources()
{
	FileIO::Load(m_filepath.c_str(), this);
}

}