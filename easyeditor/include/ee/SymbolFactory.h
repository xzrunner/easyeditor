#ifndef _EASYEDITOR_SYMBOL_FACTORY_H_
#define _EASYEDITOR_SYMBOL_FACTORY_H_

#include <map>

namespace ee
{

class Symbol;

class SymbolFactory
{
public:
	static Symbol* create(const std::string& filepath);

	typedef Symbol* (*CreateCallback)();
	static void RegisterCreator(const std::string& type, CreateCallback cb);
	static void UnregisterCreator(const std::string& type);

private:
	typedef std::map<std::string, CreateCallback> CallbackMap;
	static CallbackMap m_creators;

}; // SymbolFactory

}

#endif // _EASYEDITOR_SYMBOL_FACTORY_H_