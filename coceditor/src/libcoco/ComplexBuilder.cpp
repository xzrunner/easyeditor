#include "ComplexBuilder.h"
#include "PackAnimation.h"
#include "PackNodeFactory.h"
#include "ExportNameSet.h"
#include "PackAnchor.h"
#include "UtilityNew.h"

#include <easycomplex.h>

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

	std::map<const ecomplex::Symbol*, const PackAnchor*>::iterator
		itr2 = m_map_anchors.begin();
	for ( ; itr2 != m_map_anchors.end(); ++itr2) {
		delete itr2->second;
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
	std::map<const ecomplex::Symbol*, const PackAnchor*>::iterator
		itr2 = m_map_anchors.find(symbol);
	if (itr2 != m_map_anchors.end()) {
		return itr2->second;
	}

	std::map<const ecomplex::Symbol*, const PackAnimation*>::iterator 
		itr = m_map_data.find(symbol);
	if (itr != m_map_data.end()) {
		return itr->second;
	}

	if (symbol->m_sprites.size() == 1 && Utility::IsAnchor(symbol->m_sprites[0])) {
		LoadAnchor(symbol);

		PackAnchor* anchor = new PackAnchor;
		m_map_anchors.insert(std::make_pair(symbol, anchor));
		return anchor;
	} else {		
		return LoadComplex(symbol);
	}
}

IPackNode* ComplexBuilder::LoadComplex(const ecomplex::Symbol* symbol)
{
	PackAnimation* node = new PackAnimation;

	m_export_set.LoadExport(symbol, node);

	// todo: clipbox

	std::map<std::string, std::vector<d2d::ISprite*> > map_actions;
	std::vector<d2d::ISprite*> others;
	GroupFromTag(symbol->m_sprites, map_actions, others);
	
	if (map_actions.empty()) 
	{
		PackAnimation::Action action;
		action.size = 1;
		node->actions.push_back(action);

		PackAnimation::Frame frame;
		for (int i = 0, n = symbol->m_sprites.size(); i < n; ++i) {
			node->CreateFramePart(symbol->m_sprites[i], frame);
		}
		node->frames.push_back(frame);
	}
	else
	{
		std::map<std::string, std::vector<d2d::ISprite*> >::iterator 
			itr = map_actions.begin();
		for ( ; itr != map_actions.end(); ++itr)
		{
			PackAnimation::Action action;
			action.size = 1;
			action.name = itr->first;
			node->actions.push_back(action);

			PackAnimation::Frame frame;
			for (int i = 0, n = itr->second.size(); i < n; ++i) {
				node->CreateFramePart(itr->second[i], frame);
			}
			for (int i = 0, n = others.size(); i < n; ++i) {
				node->CreateFramePart(others[i], frame);
			}
			node->frames.push_back(frame);
		}
	}

	m_map_data.insert(std::make_pair(symbol, node));

	return node;
}

IPackNode* ComplexBuilder::LoadAnchor(const ecomplex::Symbol* symbol)
{
	assert(symbol->m_sprites.size() == 1);

	if (!Utility::IsNameValid(symbol->m_sprites[0]->name)) {
		return NULL;
	}

	PackAnimation* node = new PackAnimation;		

	m_export_set.LoadExport(symbol, node);

	PackAnimation::Action action;
	action.size = 1;
	node->actions.push_back(action);

	PackAnimation::Frame frame;
	node->CreateFramePart(symbol->m_sprites[0], frame);
	node->frames.push_back(frame);

	m_map_data.insert(std::make_pair(symbol, node));

	return node;
}

void ComplexBuilder::GroupFromTag(const std::vector<d2d::ISprite*>& src, 
								  std::map<std::string, std::vector<d2d::ISprite*> >& dst, 
								  std::vector<d2d::ISprite*>& others)
{
	for (int i = 0, n = src.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = src[i];
		if (sprite->tag.empty())
		{
			others.push_back(sprite);
		}
		else
		{
			std::vector<std::string> tags;
			d2d::StringTools::Split(sprite->tag, ";", tags);
			bool is_action = false;
			for (int i = 0, n = tags.size(); i < n; ++i)
			{
// 				if (tags[i].find("=") != std::string::npos) {
// 					continue;
// 				}

				is_action = true;

				std::map<std::string, std::vector<d2d::ISprite*> >::iterator 
					itr = dst.find(tags[i]);
				if (itr == dst.end())
				{
					std::vector<d2d::ISprite*> sprites;
					sprites.push_back(sprite);
					dst.insert(std::make_pair(tags[i], sprites));
				}
				else
				{
					itr->second.push_back(sprite);
				}
			}

			if (!is_action) {
				others.push_back(sprite);
			}
		}
	}
}

}