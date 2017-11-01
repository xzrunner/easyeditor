#include "SpriteLoader.h"
#include "SpriteFactory.h"
#include "Sprite.h"
#include "SymbolMgr.h"
#include "Exception.h"
#include "SymbolSearcher.h"

namespace ee
{

s2::SprPtr SpriteLoader::Create(const s2::SymPtr& sym) const
{
	return SpriteFactory::Instance()->Create(std::dynamic_pointer_cast<Symbol>(sym));
}

s2::SprPtr SpriteLoader::Create(const CU_STR& filepath) const
{
	auto sym = SymbolMgr::Instance()->FetchSymbol(filepath.c_str());
	if (!sym) {
		throw Exception("Symbol doesn't exist\n[file]:%s !", filepath.c_str());
	}
	return SpriteFactory::Instance()->Create(sym);
}

s2::SprPtr SpriteLoader::Create(const Json::Value& val, const CU_STR& dir) const
{
	return SpriteFactory::Instance()->Create(val, dir.c_str());
}

std::string SpriteLoader::GetSymbolPath(const std::string& dir, const Json::Value& val) const
{
	return SymbolSearcher::GetSymbolPath(dir, val);
}

}