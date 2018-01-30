#include "Symbol.h"
#include "Sprite.h"

#include <ee/SymbolLoader.h>

#include <s2loader/Anim2SymLoader.h>
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
	if (!gum::FilepathHelper::Exists(m_filepath.c_str())) {
		return false;
	}

	auto sym_loader(std::make_shared<ee::SymbolLoader>());
	s2loader::Anim2SymLoader loader(*this, sym_loader);
	loader.LoadJson(m_filepath.c_str());

	return true;
}

}