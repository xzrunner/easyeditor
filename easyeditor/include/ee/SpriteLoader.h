#ifndef _EASYEDITOR_SPRITE_LOADER_H_
#define _EASYEDITOR_SPRITE_LOADER_H_

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
	virtual s2::Sprite* Load(const Json::Value& val, const std::string& dir) const;
	virtual s2::Sprite* Load(const std::string& filepath) const;

protected:
	virtual std::string GetSymbolPath(const std::string& dir, const Json::Value& val) const;

}; // SpriteLoader

}

#endif // _EASYEDITOR_SPRITE_LOADER_H_