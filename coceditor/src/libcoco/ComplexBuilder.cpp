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

	for_each(m_anchors.begin(), m_anchors.end(), DeletePointerFunctor<const PackAnchor>());
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

	if (symbol->m_sprites.size() == 1 && Utility::IsAnchor(symbol->m_sprites[0])) {
		PackAnchor* anchor = new PackAnchor;
		m_anchors.push_back(anchor);
		return anchor;
	} else {
		PackAnimation* node = new PackAnimation;
		Load(symbol, node);
		m_map_data.insert(std::make_pair(symbol, node));
		return node;
	}
}

void ComplexBuilder::Load(const ecomplex::Symbol* symbol, PackAnimation* anim)
{
	m_export_set.LoadExport(symbol, anim);

	// todo: clipbox

	std::map<std::string, std::vector<d2d::ISprite*> > map_actions;
	std::vector<d2d::ISprite*> others;
	GroupFromTag(symbol->m_sprites, map_actions, others);
	
	if (map_actions.empty()) 
	{
		PackAnimation::Action action;
		action.size = 1;
		anim->actions.push_back(action);

		PackAnimation::Frame frame;
		for (int i = 0, n = symbol->m_sprites.size(); i < n; ++i) {
			anim->CreateFramePart(symbol->m_sprites[i], frame);
		}
		anim->frames.push_back(frame);
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
			anim->actions.push_back(action);

			PackAnimation::Frame frame;
			for (int i = 0, n = itr->second.size(); i < n; ++i) {
				anim->CreateFramePart(itr->second[i], frame);
			}
			for (int i = 0, n = others.size(); i < n; ++i) {
				anim->CreateFramePart(others[i], frame);
			}
			anim->frames.push_back(frame);
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