#ifndef _EASYSPRPACKER_EXPORT_NAME_SET_H_
#define _EASYSPRPACKER_EXPORT_NAME_SET_H_

#include <CU_Singleton.h>

#include <map>
#include <string>

namespace ee { class Symbol; }

namespace esprpacker
{

class PackNode;

class ExportNameSet
{
public:
	void Insert(const ee::Symbol* sym, PackNode* node);

	void Clear() { m_map.clear(); }

private:
	std::map<std::string, int> m_map;

	SINGLETON_DECLARATION(ExportNameSet);

}; // ExportNameSet

}

#endif // _EASYSPRPACKER_EXPORT_NAME_SET_H_