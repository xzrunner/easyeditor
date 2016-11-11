#include "Symbol.h"
#include "Sprite.h"

#include <ee/SymbolLoader.h>

#include <gum/Anim2SymLoader.h>

namespace libanim2
{

Symbol::Symbol()
{
}

Symbol::~Symbol()
{
}

void Symbol::LoadResources()
{
	ee::SymbolLoader sym_loader;
	gum::Anim2SymLoader loader(this, &sym_loader);
	loader.LoadJson(m_filepath);
}

}