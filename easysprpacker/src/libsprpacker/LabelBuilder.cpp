#include "LabelBuilder.h"
#include "PackLabel.h"
#include "typedef.h"

#include <ee/StringHelper.h>

#include <easytext.h>

#include <algorithm>

namespace esprpacker
{

SINGLETON_DEFINITION(LabelBuilder);

LabelBuilder::LabelBuilder()
{
}

LabelBuilder::~LabelBuilder()
{
	Clear();
}

void LabelBuilder::Traverse(ee::Visitor<PackNode>& visitor) const
{
	for (int i = 0, n = m_labels.size(); i < n; ++i) {
		bool has_next;
		visitor.Visit(const_cast<PackLabel*>(m_labels[i]), has_next);
		if (!has_next) {
			break;
		}
	}
}

void LabelBuilder::Clear()
{
	for_each(m_labels.begin(), m_labels.end(), cu::RemoveRefFunctor<const PackLabel>());
	m_labels.clear();
}

const PackNode* LabelBuilder::Create(const etext::Sprite* spr)
{
	for (int i = 0, n = m_labels.size(); i < n; ++i) {
		const PackLabel* label = m_labels[i];
		if (spr->GetTextbox() == label->GetTextBox() && 
			spr->GetText() == label->GetText() &&
			spr->GetTID() == label->GetTid()) {
			label->AddReference();
			return label;
		}
	}

	PackLabel* node = new PackLabel(spr);
	node->SetFilepath(SPRITE_FILEPATH);
	node->SetID(dynamic_cast<const ee::Symbol*>(spr->GetSymbol())->GetFilepath());
	m_labels.push_back(node);
	node->AddReference();
	return node;
}

void LabelBuilder::OutputExtraInfo(Json::Value& value) const
{
	for (int i = 0, n = m_labels.size(); i < n; ++i) 
	{
		const PackLabel* label = m_labels[i];
		if (label->GetText().empty() && label->GetTid().empty()) {
			continue;
		}

		Json::Value item_val;
		item_val["type"] = "label";
		item_val["id"] = label->GetID();
		if (!label->GetText().empty()) {
			item_val["content"] = ee::StringHelper::ToUtf8(label->GetText());
		}
		if (!label->GetTid().empty()) {
			item_val["tid"] = ee::StringHelper::ToUtf8(label->GetTid());
		}
		value[value.size()] = item_val;
	}
}

}