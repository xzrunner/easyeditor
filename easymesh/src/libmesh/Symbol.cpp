#include "Symbol.h"
#include "Mesh.h"
#include "FileIO.h"
#include "MeshFactory.h"

#include <gum/FilepathHelper.h>

namespace emesh
{

Symbol::Symbol()
{
}

Symbol::Symbol(const ee::SymPtr& base)
{
	m_mesh = MeshFactory::Instance()->CreateMesh(base);
}

bool Symbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath.c_str())) {
		return false;
	}
	FileIO::Load(m_filepath.c_str(), *this);
	return true;
}

}