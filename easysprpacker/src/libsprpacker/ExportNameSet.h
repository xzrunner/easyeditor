#ifndef _EASYSPRPACKER_EXPORT_NAME_SET_H_
#define _EASYSPRPACKER_EXPORT_NAME_SET_H_

#include <ee/Symbol.h>

#include <cu/cu_macro.h>

#include <map>
#include <string>

namespace esprpacker
{

class PackNode;

class ExportNameSet
{
public:
	void Insert(const ee::SymConstPtr& sym, PackNode* node);

	void Clear() { m_map.clear(); }

	const std::map<std::string, int>& GetData() const { return m_map; }

private:
	std::map<std::string, int> m_map;

	CU_SINGLETON_DECLARATION(ExportNameSet);

}; // ExportNameSet

}

#endif // _EASYSPRPACKER_EXPORT_NAME_SET_H_