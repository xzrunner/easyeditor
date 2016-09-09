#include "LayersLoader.h"
#include "Symbol.h"

#include <ee/SymbolSearcher.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/SpriteFactory.h>
#include <ee/Sprite.h>

namespace eanim
{

LayersLoader::LayersLoader(Symbol* sym)
	: glue::AnimSymLoader(sym)
{
}

s2::Sprite* LayersLoader::LoadSprite(const Json::Value& val, const std::string& dir)
{
	std::string filepath = GetSymbolPath(dir, val);
	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	if (!sym) {
		std::string filepath = val["filepath"].asString();
		throw ee::Exception("Symbol doesn't exist, \n[dir]:%s, \n[file]:%s !", 
			dir.c_str(), filepath.c_str());
	}
	ee::SymbolSearcher::SetSymbolFilepaths(dir, sym, val);
	ee::Sprite* spr = ee::SpriteFactory::Instance()->Create(sym);
	spr->Load(val, dir);
	sym->RemoveReference();
	return spr;
}

}