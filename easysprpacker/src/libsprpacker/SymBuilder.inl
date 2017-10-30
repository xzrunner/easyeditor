#ifndef _EASYSPRPACKER_SYM_BUILDER_INL_
#define _EASYSPRPACKER_SYM_BUILDER_INL_

#include "ExportNameSet.h"

namespace esprpacker
{

template <typename TSymbol, typename TPack>
SymBuilder<TSymbol, TPack>::SymBuilder(bool export_name)
	: m_export_name(export_name)
{
}

template <typename TSymbol, typename TPack>
SymBuilder<TSymbol, TPack>::~SymBuilder()
{
	Clear();
}

template <typename TSymbol, typename TPack>
void SymBuilder<TSymbol, TPack>::Traverse(ee::Visitor<PackNode>& visitor) const
{
	auto itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<TPack*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

template <typename TSymbol, typename TPack>
void SymBuilder<TSymbol, TPack>::Clear()
{
	auto itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->second->RemoveReference();
	}
	m_map_data.clear();
}

template <typename TSymbol, typename TPack>
const PackNode* SymBuilder<TSymbol, TPack>::Create(const std::shared_ptr<const TSymbol>& sym)
{
	auto itr = m_map_data.find(sym);
	if (itr != m_map_data.end()) {
		itr->second->AddReference();
		return itr->second;
	}

	TPack* node = new TPack(std::const_pointer_cast<TSymbol>(sym));
	node->SetFilepath(sym->GetFilepath());
	node->SetID(sym->GetFilepath());
	if (m_export_name) {
		ExportNameSet::Instance()->Insert(sym, node);
	}
	m_map_data.insert(std::make_pair(sym, node));
	node->AddReference();
	return node;
}

}

#endif // _EASYSPRPACKER_SYM_BUILDER_INL_
