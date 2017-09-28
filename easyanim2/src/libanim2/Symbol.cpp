#include "Symbol.h"
#include "Sprite.h"

#include <ee/SymbolLoader.h>

#include <gum/Anim2SymLoader.h>
#include <gum/FilepathHelper.h>

namespace libanim2
{

Symbol::Symbol()
{
}

Symbol::~Symbol()
{
}

bool Symbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath)) {
		return false;
	}

	ee::SymbolLoader sym_loader;
	gum::Anim2SymLoader loader(this, &sym_loader);
	loader.LoadJson(m_filepath);

	return true;
}

}