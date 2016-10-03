#include "Symbol.h"
#include "config.h"

#include <ee/StringHelper.h>
#include <ee/SpriteLoader.h>

namespace escale9
{

Symbol::Symbol()
{
	static int id = 0;
	m_name = FILE_TAG + ee::StringHelper::ToString(id++);
}

void Symbol::LoadResources()
{
	ee::SpriteLoader spr_loader;
	gum::Scale9SymLoader loader(this, &spr_loader);
	loader.LoadJson(m_filepath);
}

}