#include "s2loader/SpriteLoader.h"
#include "s2loader/SpriteFactory.h"

namespace s2loader
{

s2::SprPtr SpriteLoader::Create(const s2::SymPtr& sym) const
{
	return SpriteFactory::Instance()->Create(sym);
}

s2::SprPtr SpriteLoader::Create(const CU_STR& filepath) const
{
	return SpriteFactory::Instance()->Create(filepath);
}

s2::SprPtr SpriteLoader::Create(const Json::Value& val, const CU_STR& dir) const
{
	return SpriteFactory::Instance()->Create(val, dir);
}

s2::SprPtr SpriteLoader::Create(const s2s::NodeSpr* spr, const CU_STR& dir) const
{
	return SpriteFactory::Instance()->Create(spr, dir);
}

}