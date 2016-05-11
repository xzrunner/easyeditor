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

	//
	// Cloneable interface
	//
	virtual ScriptsSymbol* Clone() const { return NULL; }

	//
	// Symbol interfaces
	//
	virtual void Draw(const s2::RenderParams& trans, const s2::Sprite* spr = NULL) const {}
	virtual Rect GetSize(const Sprite* sprite = NULL) const;

	const std::string GetContent() const { return m_data; }

protected:
	virtual void LoadResources();

private:
	std::string m_data;

}; // ScriptsSymbol

}

#endif // _EASYEDITOR_SCRIPTS_SYMBOL_H_