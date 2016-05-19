#ifndef _EASYEDITOR_OPEN_SYMBOL_MONITOR_H_
#define _EASYEDITOR_OPEN_SYMBOL_MONITOR_H_

#include "Object.h"

namespace ee
{

class Symbol;

class OpenSymbolMonitor : public Object
{
public:
	virtual void OnOpenSymbol(Symbol* sym) = 0;
	virtual ~OpenSymbolMonitor() {}
}; // OpenSymbolMonitor

}

#endif // _EASYEDITOR_OPEN_SYMBOL_MONITOR_H_