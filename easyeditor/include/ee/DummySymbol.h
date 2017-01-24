#ifndef _EASYEDITOR_DUMMY_SYMBOL_H_
#define _EASYEDITOR_DUMMY_SYMBOL_H_

#include "Symbol.h"

#include <sprite2/DummySymbol.h>

namespace ee
{

class DummySymbol : public Symbol, public s2::DummySymbol
{
public:
	DummySymbol(const std::string& filepath);
	DummySymbol(const std::string& filepath, int width, int height);

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual void Traverse(const s2::SymVisitor& visitor) {}
	virtual sm::rect GetBounding(const s2::Sprite* spr = NULL) const;

protected:
	virtual bool LoadResources() { return true; }

private:
	sm::rect m_bounding;

}; // DummySymbol

}

#endif // _EASYEDITOR_DUMMY_SYMBOL_H_