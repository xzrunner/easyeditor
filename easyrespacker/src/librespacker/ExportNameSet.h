#ifndef _LIBRESPACKER_EXPORT_NAME_SET_H_
#define _LIBRESPACKER_EXPORT_NAME_SET_H_

#include <drag2d.h>

namespace librespacker
{

class IPackNode;

class ExportNameSet
{
public:
	
	void LoadExport(const d2d::ISymbol* symbol, IPackNode* node);

	const std::map<std::string, int>& GetData() const { return m_map; }

private:
	std::map<std::string, int> m_map;

}; // ExportNameSet

}

#endif // _LIBRESPACKER_EXPORT_NAME_SET_H_