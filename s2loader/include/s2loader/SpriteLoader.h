#ifndef _S2LOADER_SPRITE_LOADER_H_
#define _S2LOADER_SPRITE_LOADER_H_

#include <sprite2/typedef.h>
#include <cu/cu_stl.h>

#include <json/json.h>

namespace s2s { class NodeSpr; }

namespace s2loader
{

class SpriteLoader
{
public:
	virtual s2::SprPtr Create(const s2::SymPtr& sym) const;
	virtual s2::SprPtr Create(const CU_STR& filepath) const;
	virtual s2::SprPtr Create(const Json::Value& val, const CU_STR& dir) const;
	virtual s2::SprPtr Create(const s2s::NodeSpr* node, const CU_STR& dir) const;

}; // SpriteLoader

}

#endif // _S2LOADER_SPRITE_LOADER_H_
