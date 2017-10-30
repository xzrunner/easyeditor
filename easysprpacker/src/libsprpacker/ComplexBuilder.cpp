#include "ComplexBuilder.h"
#include "PackComplex.h"
#include "ExportNameSet.h"

#include <easycomplex.h>
#include <easyrespacker.h>

namespace esprpacker
{

CU_SINGLETON_DEFINITION(ComplexBuilder);

ComplexBuilder::ComplexBuilder()
{
}

ComplexBuilder::~ComplexBuilder()
{
	Clear();
}

void ComplexBuilder::Traverse(ee::Visitor<PackNode>& visitor) const
{
	auto itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackComplex*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

void ComplexBuilder::Clear()
{
	auto itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->second->RemoveReference();
	}
	m_map_data.clear();
}

const PackNode* ComplexBuilder::Create(const std::shared_ptr<const ecomplex::Symbol>& sym)
{
	auto itr = m_map_data.find(sym);
	if (itr != m_map_data.end()) {
		itr->second->AddReference();
		return itr->second;
	}

	// tag key-val
	auto& children = sym->GetAllChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		auto child = std::dynamic_pointer_cast<ee::Sprite>(children[i]);
		erespacker::PackTag::Instance()->AddTask(sym->GetFilepath(), i, child);
	}
	
	PackComplex* node = new PackComplex(sym);
	node->SetFilepath(sym->GetFilepath());
	node->SetID(sym->GetFilepath());
	ExportNameSet::Instance()->Insert(sym, node);
	m_map_data.insert(std::make_pair(sym, node));
	node->AddReference();
	return node;
}

}