#ifndef _EASYEDITOR_RELOAD_SYM_VISITOR_H_
#define _EASYEDITOR_RELOAD_SYM_VISITOR_H_

#include "Visitor.h"
#include "Symbol.h"

namespace ee
{

class ReloadSymVisitor : public Visitor<Symbol>
{
public:
	virtual void Visit(Symbol* sym, bool& next)
	{
		sym->LoadFromFile(sym->GetFilepath());
		next = true;
	}

}; // ReloadSymVisitor

}

#endif // _EASYEDITOR_RELOAD_SYM_VISITOR_H_