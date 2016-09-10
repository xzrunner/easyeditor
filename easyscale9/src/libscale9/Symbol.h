#ifndef _EASYSCALE9_SYMBOL_H_
#define _EASYSCALE9_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/Scale9Symbol.h>
#include <gum/Scale9SymLoader.h>

#include <json/json.h>

namespace escale9
{

class Symbol : public ee::Symbol, public s2::Scale9Symbol
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

private:
	class Loader : public gum::Scale9SymLoader
	{
	public:
		Loader(Symbol* sym);
	protected:
		virtual s2::Sprite* LoadSprite(const Json::Value& val, const std::string& dir);
	}; // Loader

}; // Symbol

}

#endif // _EASYSCALE9_SYMBOL_H_