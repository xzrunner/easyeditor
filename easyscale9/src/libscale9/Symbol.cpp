#include "Symbol.h"

#include <ee/StringHelper.h>
#include <ee/SpriteLoader.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>
#include <gum/StringHelper.h>

namespace escale9
{

Symbol::Symbol()
{
	static int id = 0;
	m_name = ee::SymbolFile::Instance()->Tag(s2::SYM_SCALE9) + gum::StringHelper::ToString(id++);
}

void Symbol::LoadResources()
{
	ee::SpriteLoader spr_loader;
	gum::Scale9SymLoader loader(this, &spr_loader);
	loader.LoadJson(m_filepath);
}

}