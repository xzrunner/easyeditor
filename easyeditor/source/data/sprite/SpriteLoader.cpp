#include "SpriteLoader.h"
#include "SpriteFactory.h"
#include "Sprite.h"
#include "SymbolMgr.h"
#include "Exception.h"
#include "SymbolSearcher.h"

namespace ee
{

s2::Sprite* SpriteLoader::Create(s2::Symbol* sym) const
{
	return SpriteFactory::Instance()->Create(dynamic_cast<ee::Symbol*>(sym));	
}

s2::Sprite* SpriteLoader::Create(const std::string& filepath) const
{
	Symbol* sym = SymbolMgr::Instance()->FetchSymbol(filepath);
	if (!sym) {
		throw Exception("Symbol doesn't exist\n[file]:%s !", filepath.c_str());
	}
	Sprite* spr = SpriteFactory::Instance()->Create(sym);
	sym->RemoveReference();
	return spr;
}

s2::Sprite* SpriteLoader::Create(const Json::Value& val, const std::string& dir) const
{
	return SpriteFactory::Instance()->Create(val, dir);
}

std::string SpriteLoader::GetSymbolPath(const std::string& dir, const Json::Value& val) const
{
	return SymbolSearcher::GetSymbolPath(dir, val);
}

}