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

#include <easycomplex.h>

namespace librespacker
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

	for_each(m_gen_nodes.begin(), m_gen_nodes.end(), DeletePointerFunctor<PackAnimation>());
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

	std::map<std::string, std::vector<d2d::ISprite*> > map_actions;
	std::vector<d2d::ISprite*> others;
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
		std::map<std::string, std::vector<d2d::ISprite*> >::iterator 
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

	WrapParticle(node);

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

void ComplexBuilder::WrapParticle(PackAnimation* anim)
{
	std::vector<int> types;
	types.resize(anim->components.size(), 0);

 	bool has_particle = false;
 	for (int i = 0, n = anim->components.size(); i < n; ++i) 
 	{
 		const IPackNode* child = anim->components[i].node;
		if (dynamic_cast<const PackParticle3D*>(child)) {
			has_particle = true;
			types[i] = 3;
		} else if (dynamic_cast<const PackParticle2D*>(child)) {
			has_particle = true;
			types[i] = 2;
		}
 	}
 	if (!has_particle) {
 		return;
 	}

	std::vector<PackAnimation::Part*> parts;
	for (int i = 0, n = anim->frames.size(); i < n; ++i) {
		PackAnimation::Frame* f = &anim->frames[i];
		for (int j = 0, m = f->parts.size(); j < m; ++j) {
			PackAnimation::Part* p = &f->parts[j];
			if (types[p->comp_idx] != 0) {
				parts.push_back(p);
			}
		}
	}

	if (parts.size() == 0 ||
		parts.size() == 1 && anim->components.size() > 1) {
		return;
	}

	for (int i = 0, n = parts.size(); i < n; ++i) {
		PackAnimation::Part* p = parts[i];
		assert(types[p->comp_idx] == 2 || types[p->comp_idx] == 3);

		PackAnimation* wrapper = new PackAnimation;
		wrapper->SetFilepath("[gen]");
		wrapper->components.push_back(anim->components[p->comp_idx]);
		PackAnimation::Action action;
		action.size = 1;
		wrapper->actions.push_back(action);
		PackAnimation::Part part;
		part.comp_idx = 0;
		PackAnimation::Frame frame;
		frame.parts.push_back(part);
		wrapper->frames.push_back(frame);
		m_gen_nodes.push_back(wrapper);

		PackAnimation::Component wrap_comp;
		wrap_comp.node = wrapper;
		wrap_comp.name = anim->components[p->comp_idx].name;
		p->comp_idx = anim->components.size();
		anim->components.push_back(wrap_comp);
	}

	std::vector<int> comp_id_trans;
	std::vector<PackAnimation::Component> components;
	int offset = 0;
	for (int i = 0, n = anim->components.size(); i < n; ++i) {
		if (i < types.size() && types[i] != 0) {
			comp_id_trans.push_back(-1);
			++offset;
		} else {
			comp_id_trans.push_back(i - offset);
			components.push_back(anim->components[i]);
		}
	}

	anim->components = components;
	for (int i = 0, n = anim->frames.size(); i < n; ++i) {
		PackAnimation::Frame* f = &anim->frames[i];
		for (int j = 0, m = f->parts.size(); j < m; ++j) {
			PackAnimation::Part* p = &f->parts[j];
			p->comp_idx = comp_id_trans[p->comp_idx];
			assert(p->comp_idx != -1);
		}
	}
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
				if (tags[i].find("=") != std::string::npos) {
					continue;
				}

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