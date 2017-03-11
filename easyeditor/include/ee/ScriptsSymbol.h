#ifndef _EASYEDITOR_SCRIPTS_SYMBOL_H_
#define _EASYEDITOR_SCRIPTS_SYMBOL_H_

#include "Symbol.h"

namespace ee
{

class ScriptsSymbol : public Symbol
{
public:
	ScriptsSymbol() {}
	virtual ~ScriptsSymbol() {}

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const s2::SymbolVisitor& visitor) {}
	virtual void Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const {}
	virtual sm::rect GetBounding(const s2::Sprite* spr = NULL) const { return sm::rect(0, 0); }

	const std::string GetContent() const { return m_data; }

protected:
	virtual bool LoadResources();

private:
	std::string m_data;

}; // ScriptsSymbol

}

#endif // _EASYEDITOR_SCRIPTS_SYMBOL_H_