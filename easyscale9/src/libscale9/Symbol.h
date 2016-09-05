#ifndef _EASYSCALE9_SYMBOL_H_
#define _EASYSCALE9_SYMBOL_H_

#include <ee/Symbol.h>
#include <glue/Scale9Symbol.h>

#include <json/json.h>

namespace escale9
{

class Symbol : public ee::Symbol, public glue::Scale9Symbol
{
public:
	Symbol();

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual void LoadResources();

	/**
	 *  @interface
	 *    glue::Scale9Symbol
	 */
	virtual s2::Sprite* LoadSprite(const Json::Value& val, const std::string& dir);

}; // Symbol

}

#endif // _EASYSCALE9_SYMBOL_H_