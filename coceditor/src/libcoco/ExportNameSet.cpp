#include "ExportNameSet.h"
#include "IPackNode.h"
#include "UtilityNew.h"

namespace libcoco
{

void ExportNameSet::LoadExport(const d2d::ISymbol* symbol, IPackNode* node)
{
	std::string export_name = symbol->name;
	if (!Utility::IsNameValid(export_name)) {
		return;
	}

	std::map<std::string, int>::iterator itr = m_map.find(export_name);
	if (itr != m_map.end()) {
		throw d2d::Exception("Duplicate export name %s, file %s", 
			export_name.c_str(), symbol->GetFilepath().c_str());
	}

	node->export_name = export_name;
	m_map.insert(std::make_pair(export_name, node->GetID()));
}

}