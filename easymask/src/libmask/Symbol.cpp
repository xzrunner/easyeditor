#include "Symbol.h"
#include "FileIO.h"

#include <sprite2/S2_Sprite.h>
#include <gum/FilepathHelper.h>

namespace emask
{

bool Symbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath)) {
		return false;
	}
	FileIO::Load(m_filepath.c_str(), this);
	return true;
}

}