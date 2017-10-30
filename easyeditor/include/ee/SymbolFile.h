#ifndef _EASYEDITOR_SYMBOL_FILE_H_
#define _EASYEDITOR_SYMBOL_FILE_H_

#include <cu/cu_macro.h>

#include <string>
#include <map>

namespace ee
{

class SymbolFile
{
public:
	int Type(const std::string& filepath) const;

	std::string Tag(int type) const;

private:
	void Regist(int type, const std::string& tag);

	int  CheckTypeFromData(const std::string& filepath) const;

protected:
	std::map<int, std::string> m_type2tag;
	std::map<std::string, int> m_tag2type;

	CU_SINGLETON_DECLARATION(SymbolFile);

}; // SymbolFile

}

#endif // _EASYEDITOR_SYMBOL_FILE_H_