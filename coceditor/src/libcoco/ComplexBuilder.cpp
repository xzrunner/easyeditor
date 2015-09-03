#include "ComplexBuilder.h"
#include "PackAnimation.h"
#include "PackNodeFactory.h"
#include "ExportNameSet.h"

namespace libcoco
{

ComplexBuilder::ComplexBuilder(ExportNameSet& export_set)
	: m_export_set(export_set)
{
}

ComplexBuilder::~ComplexBuilder()
{
	std::map<const ecomplex::Symbol*, const PackAnimation*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second;
	}
}

void ComplexBuilder::Traverse(d2d::IVisitor& visitor) const
{
	std::map<const ecomplex::Symbol*, const PackAnimation*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackAnimation*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

const IPackNode* ComplexBuilder::Create(const ecomplex::Symbol* symbol)
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

void ComplexBuilder::Load(const ecomplex::Symbol* symbol, PackAnimation* anim)
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