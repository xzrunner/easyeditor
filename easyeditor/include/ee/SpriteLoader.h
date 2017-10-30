#ifndef _EASYEDITOR_SPRITE_LOADER_H_
#define _EASYEDITOR_SPRITE_LOADER_H_

#include "Sprite.h"
#include "Symbol.h"

#include <gum/SpriteLoader.h>

#include <json/json.h>

namespace s2 { class Sprite; }

namespace ee
{

class Symbol;

class SpriteLoader : public gum::SpriteLoader
{
public:
	/**
	 *  @interface
	 *    gum::SpriteLoader
	 */	
	virtual s2::SprPtr Create(const s2::SymPtr& sym) const;
	virtual s2::SprPtr Create(const std::string& filepath) const;
	virtual s2::SprPtr Create(const Json::Value& val, const std::string& dir) const;

protected:
	virtual std::string GetSymbolPath(const std::string& dir, const Json::Value& val) const;

}; // SpriteLoader

}

#endif // _EASYEDITOR_SPRITE_LOADER_H_