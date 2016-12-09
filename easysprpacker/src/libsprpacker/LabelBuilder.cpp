#include "LabelBuilder.h"
#include "PackLabel.h"
#include "typedef.h"

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
	for (int i = 0, n = m_data.size(); i < n; ++i) {
		bool has_next;
		visitor.Visit(const_cast<PackLabel*>(m_data[i]), has_next);
		if (!has_next) {
			break;
		}
	}
}

void LabelBuilder::Clear()
{
	for_each(m_data.begin(), m_data.end(), cu::RemoveRefFunctor<const PackLabel>());
	m_data.clear();
}

const PackNode* LabelBuilder::Create(const etext::Sprite* spr)
{
	for (int i = 0, n = m_data.size(); i < n; ++i) {
		const PackLabel* label = m_data[i];
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
	m_data.push_back(node);
	node->AddReference();
	return node;
}

}