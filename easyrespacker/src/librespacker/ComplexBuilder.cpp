#include "ComplexBuilder.h"
#include "PackAnimation.h"
#include "PackNodeFactory.h"
#include "PackClipbox.h"
#include "ExportNameSet.h"
#include "Utility.h"
#include "ClipboxBuilder.h"
#include "PackUI.h"
#include "PackTag.h"

#include "TextBuilder.h"

#include "PackParticle3D.h"
#include "PackParticle2D.h"

#include <ee/std_functor.h>
#include <ee/Visitor.h>
#include <ee/StringHelper.h>

#include <easycomplex.h>

#include <sprite2/Sprite.h>

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
	auto itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second;
	}

	for_each(m_gen_nodes.begin(), m_gen_nodes.end(), ee::DeletePointerFunctor<PackAnimation>());
}

void ComplexBuilder::Traverse(ee::Visitor<IPackNode>& visitor) const
{
	auto itr = m_map_data.begin();
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

const IPackNode* ComplexBuilder::Create(const std::shared_ptr<const ecomplex::Symbol>& sym)
{
	auto itr = m_map_data.find(sym);
	if (itr != m_map_data.end()) {
		return itr->second;
	}

	PackNodeFactory::Instance()->GetTextBuilder()->CacheBegin();
	IPackNode* node = LoadComplex(sym);
	PackNodeFactory::Instance()->GetTextBuilder()->CacheEnd();

	PackUI::Instance()->OnKnownPackID(sym->GetFilepath(), node->GetSprID());
	PackTag::Instance()->OnKnownPackID(sym->GetFilepath(), node->GetSprID());	

	return node;
}

IPackNode* ComplexBuilder::LoadComplex(const std::shared_ptr<const ecomplex::Symbol>& sym)
{
	PackAnimation* node = new PackAnimation;

	auto& children = sym->GetAllChildren();

	m_export_set.LoadExport(sym, node);

	// clipbox
	const PackClipbox* cb = static_cast<const PackClipbox*>(m_cb_builder->Create(sym));

	// tag key-val
	for (int i = 0, n = children.size(); i < n; ++i) {
		auto child = std::dynamic_pointer_cast<ee::Sprite>(children[i]);
		PackTag::Instance()->AddTask(sym->GetFilepath(), i, child);
	}

	// actions
	std::map<std::string, std::vector<ee::SprPtr> > map_actions;
	std::vector<ee::SprPtr> others;
	GroupFromTag(children, map_actions, others);
	
	if (map_actions.empty()) 
	{
		PackAnimation::Action action;
		action.size = 1;
		node->actions.push_back(action);

		PackAnimation::Frame frame;
		if (cb) {
			node->CreateClipboxFramePart(cb, frame);
		}
		for (int i = 0, n = children.size(); i < n; ++i) {
			auto child = std::dynamic_pointer_cast<ee::Sprite>(children[i]);
			node->CreateFramePart(child, frame);
		}
		node->frames.push_back(frame);
	}
	else
	{
		std::map<std::string, std::vector<ee::SprPtr> >::iterator 
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

	m_map_data.insert(std::make_pair(sym, node));

	return node;
}

IPackNode* ComplexBuilder::LoadAnchor(const std::shared_ptr<ecomplex::Symbol>& sym)
{
	auto& children = sym->GetAllChildren();
	assert(children.size() == 1);

	auto child = std::dynamic_pointer_cast<ee::Sprite>(children[0]);
	CU_STR name;
	s2::SprNameMap::Instance()->IDToStr(child->GetName(), name);
	if (!Utility::IsNameValid(name.c_str())) {
		return NULL;
	}

	PackAnimation* node = new PackAnimation;		

	m_export_set.LoadExport(sym, node);

	PackAnimation::Action action;
	action.size = 1;
	node->actions.push_back(action);

	PackAnimation::Frame frame;
	node->CreateFramePart(child, frame);
	node->frames.push_back(frame);

	m_map_data.insert(std::make_pair(sym, node));

	return node;
}

void ComplexBuilder::GroupFromTag(const CU_VEC<s2::SprPtr>& src, 
								  std::map<std::string, std::vector<ee::SprPtr> >& dst, 
								  std::vector<ee::SprPtr>& others)
{
	for (int i = 0, n = src.size(); i < n; ++i)
	{
		auto& spr = std::dynamic_pointer_cast<ee::Sprite>(src[i]);
		if (spr->GetTag().empty())
		{
			others.push_back(spr);
		}
		else
		{
			std::vector<std::string> tags;
			ee::StringHelper::Split(spr->GetTag(), ";", tags);
			bool is_action = false;
			for (int i = 0, n = tags.size(); i < n; ++i)
			{
				if (tags[i].find("=") != std::string::npos) {
					continue;
				}

				is_action = true;

				std::map<std::string, std::vector<ee::SprPtr> >::iterator 
					itr = dst.find(tags[i]);
				if (itr == dst.end())
				{
					std::vector<ee::SprPtr> sprs;
					sprs.push_back(spr);
					dst.insert(std::make_pair(tags[i], sprs));
				}
				else
				{
					itr->second.push_back(spr);
				}
			}

			if (!is_action) {
				others.push_back(spr);
			}
		}
	}
}

}