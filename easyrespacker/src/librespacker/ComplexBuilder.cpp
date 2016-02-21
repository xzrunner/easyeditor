#include "ComplexBuilder.h"
#include "PackAnimation.h"
#include "PackNodeFactory.h"
#include "PackClipbox.h"
#include "ExportNameSet.h"
#include "Utility.h"
#include "ClipboxBuilder.h"

#include "TextBuilder.h"

#include "PackParticle3D.h"
#include "PackParticle2D.h"

#include <ee/std_functor.h>
#include <ee/Visitor.h>
#include <ee/StringHelper.h>

#include <easycomplex.h>

#include <algorithm>

namespace erespacker
{

ComplexBuilder::ComplexBuilder(ExportNameSet& export_set, ClipboxBuilder* cb_builder)
	: m_export_set(export_set)
	, m_cb_builder(cb_builder)
{
}

ComplexBuilder::~ComplexBuilder()
{
	std::map<const ecomplex::Symbol*, const PackAnimation*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second;
	}

	for_each(m_gen_nodes.begin(), m_gen_nodes.end(), ee::DeletePointerFunctor<PackAnimation>());
}

void ComplexBuilder::Traverse(ee::Visitor& visitor) const
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

	for (int i = 0, n = m_gen_nodes.size(); i < n; ++i) {
		bool has_next;
		visitor.Visit(const_cast<PackAnimation*>(m_gen_nodes[i]), has_next);
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

	PackNodeFactory::Instance()->GetTextBuilder()->CacheBegin();
	IPackNode* node = LoadComplex(symbol);
	PackNodeFactory::Instance()->GetTextBuilder()->CacheEnd();
	return node;
}

IPackNode* ComplexBuilder::LoadComplex(const ecomplex::Symbol* symbol)
{
	PackAnimation* node = new PackAnimation;

	m_export_set.LoadExport(symbol, node);

	// clipbox
	const PackClipbox* cb = static_cast<const PackClipbox*>(m_cb_builder->Create(symbol));

	std::map<std::string, std::vector<ee::Sprite*> > map_actions;
	std::vector<ee::Sprite*> others;
	GroupFromTag(symbol->m_sprites, map_actions, others);
	
	if (map_actions.empty()) 
	{
		PackAnimation::Action action;
		action.size = 1;
		node->actions.push_back(action);

		PackAnimation::Frame frame;
		if (cb) {
			node->CreateClipboxFramePart(cb, frame);
		}
		for (int i = 0, n = symbol->m_sprites.size(); i < n; ++i) {
			node->CreateFramePart(symbol->m_sprites[i], frame);
		}
		node->frames.push_back(frame);
	}
	else
	{
		std::map<std::string, std::vector<ee::Sprite*> >::iterator 
			itr = map_actions.begin();
		for ( ; itr != map_actions.end(); ++itr)
		{
			PackAnimation::Action action;
			action.size = 1;
			action.name = itr->first;
			node->actions.push_back(action);

			PackAnimation::Frame frame;
			if (cb) {
				node->CreateClipboxFramePart(cb, frame);
			}
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

void ComplexBuilder::GroupFromTag(const std::vector<ee::Sprite*>& src, 
								  std::map<std::string, std::vector<ee::Sprite*> >& dst, 
								  std::vector<ee::Sprite*>& others)
{
	for (int i = 0, n = src.size(); i < n; ++i)
	{
		ee::Sprite* sprite = src[i];
		if (sprite->tag.empty())
		{
			others.push_back(sprite);
		}
		else
		{
			std::vector<std::string> tags;
			ee::StringHelper::Split(sprite->tag, ";", tags);
			bool is_action = false;
			for (int i = 0, n = tags.size(); i < n; ++i)
			{
				if (tags[i].find("=") != std::string::npos) {
					continue;
				}

				is_action = true;

				std::map<std::string, std::vector<ee::Sprite*> >::iterator 
					itr = dst.find(tags[i]);
				if (itr == dst.end())
				{
					std::vector<ee::Sprite*> sprites;
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