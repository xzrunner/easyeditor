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
	void SetImage(const std::shared_ptr<ee::ImageSymbol>& img);

	static ee::SymPtr Create() { return std::make_shared<Symbol>(); }

protected:
	virtual bool LoadResources();

}; // Symbol

}

#endif // _EASYICON_SYMBOL_H_