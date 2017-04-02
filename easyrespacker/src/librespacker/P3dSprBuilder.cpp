#include "P3dSprBuilder.h"
#include "IPackNode.h"
#include "PackNodeFactory.h"
#include "PackUI.h"
#include "PackTag.h"

#include <ee/std_functor.h>
#include <ee/Visitor.h>

#include <easyparticle3d.h>

#include <algorithm>

namespace erespacker
{

P3dSprBuilder::P3dSprBuilder(ExportNameSet& export_set)
	: m_export_set(export_set)
{
}

P3dSprBuilder::~P3dSprBuilder()
{
//	for_each(m_nodes.begin(), m_nodes.end(), ee::DeletePointerFunctor<IPackNode>());	

	std::map<const eparticle3d::Symbol*, const PackP3dSpr*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second;
	}
}

void P3dSprBuilder::Traverse(ee::Visitor<IPackNode>& visitor) const
{
// 	for (int i = 0, n = m_nodes.size(); i < n; ++i) {
// 		bool has_next;
// 		visitor.Visit(m_nodes[i], has_next);
// 		if (!has_next) {
// 			break;
// 		}
// 	}

	std::map<const eparticle3d::Symbol*, const PackP3dSpr*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackP3dSpr*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

const IPackNode* P3dSprBuilder::Create(const eparticle3d::Sprite* spr)
{
	const eparticle3d::Symbol* sym = dynamic_cast<const eparticle3d::Symbol*>(spr->GetSymbol());
	std::map<const eparticle3d::Symbol*, const PackP3dSpr*>::iterator
		itr = m_map_data.find(sym);
	if (itr != m_map_data.end()) {
		return itr->second;
	}

	PackP3dSpr* node = NULL;

	const IPackNode* p3d = PackNodeFactory::Instance()->Create(dynamic_cast<const ee::Symbol*>(spr->GetSymbol()));
	itr = m_map_data.find(sym);
	if (itr == m_map_data.end()) {
		node = new PackP3dSpr;
		m_map_data.insert(std::make_pair(sym, node));
	} else {
		node = const_cast<PackP3dSpr*>(itr->second);
	}

	node->p3d = PackNodeFactory::Instance()->Create(dynamic_cast<const ee::Symbol*>(spr->GetSymbol()));
	node->loop = spr->IsLoop();
	node->local = spr->IsLocal();
	node->alone = spr->IsAlone();
//	node->reuse = spr->IsReuse();
	node->reuse = false;

	return node;
}

//const IPackNode* P3dSprBuilder::Create(const eparticle3d::Sprite* spr)
//{
//	PackP3dSpr* node = new PackP3dSpr;
//	node->p3d = PackNodeFactory::Instance()->Create(dynamic_cast<const ee::Symbol*>(spr->GetSymbol()));
//	node->loop = spr->IsLoop();
//	node->local = spr->IsLocalModeDraw();
//	node->alone = spr->IsAlone();
//	node->reuse = spr->IsReuse();
//	m_nodes.push_back(node);
//	return node;
//}

void P3dSprBuilder::Create(const eparticle3d::Symbol* sym, const IPackNode* p3d)
{
	std::map<const eparticle3d::Symbol*, const PackP3dSpr*>::iterator
		itr = m_map_data.find(sym);
	if (itr != m_map_data.end()) {
		m_export_set.LoadExport(sym, const_cast<PackP3dSpr*>(itr->second));
		return;
	}

	PackP3dSpr* node = new PackP3dSpr;

	node->p3d = p3d;
	node->loop = sym->IsLoop();
	node->local = sym->IsLocal();
	node->alone = false;
	node->reuse = false;
	m_map_data.insert(std::make_pair(sym, node));

	PackUI::Instance()->OnKnownPackID(sym->GetFilepath(), node->GetSprID());
	PackTag::Instance()->OnKnownPackID(sym->GetFilepath(), node->GetSprID());	

	m_export_set.LoadExport(sym, node);
}

//void P3dSprBuilder::Create(const eparticle3d::Symbol* sym, const IPackNode* p3d)
//{
//	PackP3dSpr* node = new PackP3dSpr;
//
//	node->p3d = p3d;
//	node->loop = sym->IsLoop();
//	node->local = sym->IsLocal();
//	node->alone = false;
//	node->reuse = false;
//	m_nodes.push_back(node);
//
//	m_export_set.LoadExport(sym, node);
//}

}