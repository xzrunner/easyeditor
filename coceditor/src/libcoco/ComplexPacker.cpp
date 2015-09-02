#include "ComplexPacker.h"
#include "PackAnimation.h"
#include "PackNodeFactory.h"
#include "ExportNameSet.h"

namespace libcoco
{

ComplexPacker::ComplexPacker(ExportNameSet& export_set)
	: m_export_set(export_set)
{
}

ComplexPacker::~ComplexPacker()
{
	std::map<const ecomplex::Symbol*, const PackAnimation*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second;
	}
}

void ComplexPacker::PackToLuaString(ebuilder::CodeGenerator& gen,
							  const TexturePacker& tp) const
{
	std::vector<const IPackNode*> nodes;
	nodes.reserve(m_map_data.size());

	std::map<const ecomplex::Symbol*, const PackAnimation*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		nodes.push_back(itr->second);
	}

	std::sort(nodes.begin(), nodes.end(), PackNodeCmp());
	for (int i = 0, n = nodes.size(); i < n; ++i) {
		nodes[i]->PackToLuaString(gen, tp);
	}
}

const IPackNode* ComplexPacker::Create(const ecomplex::Symbol* symbol)
{
	std::map<const ecomplex::Symbol*, const PackAnimation*>::iterator 
		itr = m_map_data.find(symbol);
	if (itr != m_map_data.end()) {
		return itr->second;
	}

	PackAnimation* node = new PackAnimation;
	Load(symbol, node);
	m_map_data.insert(std::make_pair(symbol, node));
	return node;
}

void ComplexPacker::Load(const ecomplex::Symbol* symbol, PackAnimation* anim)
{
	m_export_set.LoadExport(symbol, anim);

	// todo: clipbox

	// todo: mount node

  	// todo: only one action

	PackAnimation::Action action;
  	action.size = 1;
	anim->actions.push_back(action);

	PackAnimation::Frame frame;
	for (int i = 0, n = symbol->m_sprites.size(); i < n; ++i) {
		anim->CreateFramePart(symbol->m_sprites[i], frame);
	}
	anim->frames.push_back(frame);
}

}