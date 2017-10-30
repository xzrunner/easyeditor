#ifndef _EASYEDITOR_SYMBOL_FACTORY_H_
#define _EASYEDITOR_SYMBOL_FACTORY_H_

#include "Symbol.h"

#include <string>
#include <sprite2/SymType.h>

#include <map>

namespace ee
{

class SymbolFactory
{
public:
	static SymPtr Create(const std::string& filepath, int type = s2::SYM_UNKNOWN);
	static SymPtr Create(int type);

	typedef SymPtr (*CreateCallback)();
	static void RegisterCreator(int type, CreateCallback cb);
	static void UnregisterCreator(int type);

private:
	typedef std::map<int, CreateCallback> CallbackMap;
	static CallbackMap m_creators;

}; // SymbolFactory

}

#endif // _EASYEDITOR_SYMBOL_FACTORY_H_