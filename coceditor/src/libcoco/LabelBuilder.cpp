#include "LabelBuilder.h"
#include "PackLabel.h"

namespace libcoco
{

LabelBuilder::LabelBuilder()
{
}

LabelBuilder::~LabelBuilder()
{
	for_each(m_labels.begin(), m_labels.end(), DeletePointerFunctor<const PackLabel>());
}

void LabelBuilder::ToString(ebuilder::CodeGenerator& gen,
							const TexturePacker& tp) const
{
	for (int i = 0, n = m_labels.size(); i < n; ++i) {
		m_labels[i]->ToString(gen, tp);
	}
}

const IPackNode* LabelBuilder::Create(const d2d::FontSprite* spr)
{
	for (int i = 0, n = m_labels.size(); i < n; ++i) 
	{
		const PackLabel* label = m_labels[i];
		if (label->name == spr->name &&
			label->font == spr->font &&
			label->align_hori == spr->align_hori &&
			label->align_vert == spr->align_vert && 
			label->color == spr->color &&
			label->size == spr->size &&
			label->width == spr->width &&
			label->height == spr->height &&
			label->has_edge == spr->has_edge) {
			return label;
		}
	} 

	PackLabel* node = new PackLabel;
	
	node->name = spr->name;
	node->font = spr->font;

	node->align_hori = spr->align_hori;
	node->align_vert = spr->align_vert;

	node->color = spr->color;

	node->size = spr->size;
	node->width = spr->width;
	node->height = spr->height;

	node->has_edge = spr->has_edge;

	m_labels.push_back(node);

	return node;
}

}