#include "Symbol.h"
#include "FileIO.h"

#include <sprite2/S2_Sprite.h>

namespace emask
{

void Symbol::LoadResources()
{
	FileIO::Load(m_filepath.c_str(), this);
}

}