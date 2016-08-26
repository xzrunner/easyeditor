#include "Symbol.h"
#include "FileIO.h"

namespace emask
{

void Symbol::LoadResources()
{
	FileIO::Load(m_filepath.c_str(), this);
}

}