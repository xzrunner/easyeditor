#include "ExportNameSet.h"
#include "IPackNode.h"
#include "Utility.h"

#include <ee/Exception.h>

namespace erespacker
{

void ExportNameSet::LoadExport(const ee::SymConstPtr& sym, IPackNode* node)
{
	std::string export_name = sym->name;
	if (!Utility::IsNameValid(export_name)) {
		return;
	}

	std::map<std::string, int>::iterator itr = m_map.find(export_name);
	if (itr != m_map.end()) {
		throw ee::Exception("Duplicate export name %s, file %s", 
			export_name.c_str(), sym->GetFilepath().c_str());
	}

	node->export_name = export_name;
	m_map.insert(std::make_pair(export_name, node->GetSprID()));
}

}