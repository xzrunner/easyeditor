#include "TextBuilder.h"
#include "PackLabel.h"

#include <ee/std_functor.h>
#include <ee/Visitor.h>
#include <ee/StringHelper.h>

#include <easytext.h>

#include <algorithm>

namespace erespacker
{

TextBuilder::TextBuilder()
	: m_cache(false)
{
}

TextBuilder::~TextBuilder()
{
	for_each(m_labels.begin(), m_labels.end(), ee::DeletePointerFunctor<const PackLabel>());
}

void TextBuilder::Traverse(ee::Visitor<IPackNode>& visitor) const
{
 	for (int i = 0, n = m_labels.size(); i < n; ++i) {
		bool has_next;
		visitor.Visit(const_cast<PackLabel*>(m_labels[i]), has_next);
		if (!has_next) {
			break;
		}
 	}
}

const IPackNode* TextBuilder::Create(const etext::Sprite* spr)
{
	if (m_cache) {
		const IPackNode* node = QueryCache(spr);
		if (node) {
			return node;
		}
	}

	PackLabel* node = new PackLabel;

	const s2::Textbox& tb = spr->GetTextbox();

	node->width			= tb.width;
	node->height		= tb.height;

	node->font			= tb.font_type;
	node->font_size		= tb.font_size;
	node->font_color	= tb.font_color;

	node->edge			= tb.has_edge;
	node->edge_size		= tb.edge_size;
	node->edge_color	= tb.edge_color;

	node->align_hori	= tb.align_hori;
	node->align_vert	= tb.align_vert;

	node->space_hori	= tb.space_hori;
	node->space_vert	= tb.space_vert;

	node->overflow		= tb.overflow;

	node->richtext		= tb.richtext;

	node->text			= spr->GetText();
	node->tid			= spr->GetTID();

	m_labels.push_back(node);

	if (m_cache) {
		m_cache_labels.push_back(node);
	}

	return node;
}

void TextBuilder::OutputExtraInfo(Json::Value& value) const
{
	for (int i = 0, n = m_labels.size(); i < n; ++i) {
		const PackLabel* label = m_labels[i];
		if (label->text.empty() && label->tid.empty()) {
			continue;
		}

		Json::Value item_val;
		item_val["type"] = "label";
		item_val["id"] = label->GetSprID();
		if (!label->text.empty()) {
			item_val["content"] = ee::StringHelper::ToUtf8(label->text);
		}
		if (!label->tid.empty()) {
			item_val["tid"] = ee::StringHelper::ToUtf8(label->tid);
		}
		value[value.size()] = item_val;
	}
}

void TextBuilder::CacheBegin()
{
	m_cache = true;
}

void TextBuilder::CacheEnd()
{
	m_cache = false;
	m_cache_labels.clear();
}

const IPackNode* TextBuilder::QueryCache(const etext::Sprite* spr) const
{
	const s2::Textbox& tb = spr->GetTextbox();
	for (int i = 0, n = m_cache_labels.size(); i < n; ++i) {
 		const PackLabel* label = m_cache_labels[i];
 		if (label->width		== tb.width &&
 			label->height		== tb.height &&
 
 			label->font			== tb.font_type &&
 			label->font_size	== tb.font_size &&
 			label->font_color	== tb.font_color &&
 
 			label->edge			== tb.has_edge &&
 			label->edge_size	== tb.edge_size &&
 			label->edge_color	== tb.edge_color &&
 
 			label->align_hori	== tb.align_hori &&
 			label->align_vert	== tb.align_vert &&
 
 			label->space_hori	== tb.space_hori &&
 			label->space_vert	== tb.space_vert &&

			label->overflow	    == tb.overflow &&
			
			label->richtext		== tb.richtext&&

 			label->text			== spr->GetText() &&
 			label->tid			== spr->GetTID()) {
 			return label;
 		}
	}

	return NULL;
}

}