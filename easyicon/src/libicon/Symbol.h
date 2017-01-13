#ifndef _EASYICON_SYMBOL_H_
#define _EASYICON_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/IconSymbol.h>

namespace ee { class ImageSymbol; }

namespace eicon
{

class Icon;

class Symbol : public ee::Symbol, public s2::IconSymbol
{
public:
	void SetImage(ee::ImageSymbol* img);

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual bool LoadResources();

}; // Symbol

}

#endif // _EASYICON_SYMBOL_H_