#include "Symbol.h"
#include "config.h"

#include <ee/StringHelper.h>
#include <ee/SpriteFactory.h>
#include <ee/Sprite.h>

namespace escale9
{

Symbol::Symbol()
{
	static int id = 0;
	m_name = FILE_TAG + ee::StringHelper::ToString(id++);
}

void Symbol::LoadResources()
{
	Loader loader(this);
	loader.LoadJson(m_filepath);
}

/************************************************************************/
/* class Symbol::Loader                                                 */
/************************************************************************/

Symbol::Loader::Loader(Symbol* sym)
	: gum::Scale9SymLoader(sym)
{
}

s2::Sprite* Symbol::Loader::LoadSprite(const Json::Value& val, const std::string& dir)
{
	return ee::SpriteFactory::Instance()->Create(val, dir);
}

}