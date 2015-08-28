#include "ExportNameSet.h"
#include "PackAnimation.h"

namespace libcoco
{

void ExportNameSet::LoadExport(const d2d::ISymbol* symbol, PackAnimation* anim)
{
	std::string export_name = symbol->name;
	if (export_name.empty() || export_name[0] == '_') {
		return;
	}

	std::set<std::string>::iterator itr = m_set.find(export_name);
	if (itr != m_set.end()) {
		throw d2d::Exception("Duplicate export name %s, file %s", 
			export_name.c_str(), symbol->GetFilepath().c_str());
	}

	anim->export_name = export_name;
	m_set.insert(export_name);
}

}