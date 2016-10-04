#ifndef _EASYEDITOR_SYMBOL_FACTORY_H_
#define _EASYEDITOR_SYMBOL_FACTORY_H_

#include <sprite2/SymType.h>

#include <map>

namespace ee
{

class Symbol;

class SymbolFactory
{
public:
	static Symbol* Create(const std::string& filepath, int type = s2::SYM_UNKNOWN);

	typedef Symbol* (*CreateCallback)();
	static void RegisterCreator(int type, CreateCallback cb);
	static void UnregisterCreator(int type);

private:
	typedef std::map<int, CreateCallback> CallbackMap;
	static CallbackMap m_creators;

}; // SymbolFactory

}

#endif // _EASYEDITOR_SYMBOL_FACTORY_H_