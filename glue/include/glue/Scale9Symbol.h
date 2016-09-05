#ifndef _GLUE_SCALE9_SYMBOL_H_
#define _GLUE_SCALE9_SYMBOL_H_

#include <sprite2/Scale9Symbol.h>

#include <json/json.h>

#include <string>

namespace glue
{

class Scale9Symbol : public s2::Scale9Symbol
{
public:
//	Scale9Symbol();

	void LoadJson(const std::string& filepath);

protected:
	virtual s2::Sprite* LoadSprite(const Json::Value& val, const std::string& dir);

}; // Scale9Symbol

}

#endif // _GLUE_SCALE9_SYMBOL_H_