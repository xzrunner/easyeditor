#ifndef _EASYEDITOR_OPEN_SYMBOL_MONITOR_H_
#define _EASYEDITOR_OPEN_SYMBOL_MONITOR_H_

#include "Symbol.h"

#include <cu/CU_RefCountObj.h>

namespace ee
{

class OpenSymbolMonitor : public cu::RefCountObj
{
public:
	virtual void OnOpenSymbol(const SymPtr& sym) = 0;
	virtual ~OpenSymbolMonitor() {}
}; // OpenSymbolMonitor

}

#endif // _EASYEDITOR_OPEN_SYMBOL_MONITOR_H_