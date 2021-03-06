#ifndef _EASYEDITOR_SPRITE_LOADER_H_
#define _EASYEDITOR_SPRITE_LOADER_H_

#include "Sprite.h"
#include "Symbol.h"

#include <s2loader/SpriteLoader.h>

#include <json/json.h>

namespace s2 { class Sprite; }

namespace ee
{

class Symbol;

class SpriteLoader : public s2loader::SpriteLoader
{
public:
	/**
	 *  @interface
	 *    s2loader::SpriteLoader
	 */	
	virtual s2::SprPtr Create(const s2::SymPtr& sym) const override;
	virtual s2::SprPtr Create(const CU_STR& filepath) const override;
	virtual s2::SprPtr Create(const Json::Value& val, const CU_STR& dir) const override;
	virtual s2::SprPtr Create(const s2s::NodeSpr* node, const CU_STR& dir) const override;

protected:
	virtual std::string GetSymbolPath(const std::string& dir, const Json::Value& val) const;

}; // SpriteLoader

}

#endif // _EASYEDITOR_SPRITE_LOADER_H_