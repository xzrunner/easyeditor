#include "SpriteLoader.h"
#include "SymbolSearcher.h"
#include "SymbolMgr.h"
#include "Exception.h"
#include "SpriteFactory.h"
#include "Sprite.h"

namespace ee
{

s2::Sprite* SpriteLoader::Load(const Json::Value& val, const std::string& dir) const
{
	std::string filepath = GetSymbolPath(dir, val);
	Symbol* sym = SymbolMgr::Instance()->FetchSymbol(filepath);
	if (!sym) {
		std::string filepath = val["filepath"].asString();
		throw Exception("Symbol doesn't exist, \n[dir]:%s, \n[file]:%s !", 
			dir.c_str(), filepath.c_str());
	}
	SymbolSearcher::SetSymbolFilepaths(dir, sym, val);
	Sprite* spr = SpriteFactory::Instance()->Create(sym);
	spr->Load(val, dir);
	sym->RemoveReference();
	return spr;
}

s2::Sprite* SpriteLoader::Load(const std::string& filepath) const
{
	Symbol* sym = SymbolMgr::Instance()->FetchSymbol(filepath);
	if (!sym) {
		throw Exception("Symbol doesn't exist\n[file]:%s !", filepath.c_str());
	}
	Sprite* spr = SpriteFactory::Instance()->Create(sym);
	sym->RemoveReference();
	return spr;
}

std::string SpriteLoader::GetSymbolPath(const std::string& dir, const Json::Value& val) const
{
	return SymbolSearcher::GetSymbolPath(dir, val);
}

}