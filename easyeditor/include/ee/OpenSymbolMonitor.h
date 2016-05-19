#ifndef _EASYEDITOR_OPEN_SYMBOL_MONITOR_H_
#define _EASYEDITOR_OPEN_SYMBOL_MONITOR_H_

namespace ee
{

class Symbol;

class OpenSymbolMonitor
{
public:
	virtual void OnOpenSymbol(Symbol* sym) = 0;
	virtual ~OpenSymbolMonitor() {}
}; // OpenSymbolMonitor

}

#endif // _EASYEDITOR_OPEN_SYMBOL_MONITOR_H_