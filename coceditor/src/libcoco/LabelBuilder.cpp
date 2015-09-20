#include "LabelBuilder.h"
#include "PackLabel.h"
#include "PackAnchor.h"

namespace libcoco
{

LabelBuilder::LabelBuilder()
{
}

LabelBuilder::~LabelBuilder()
{
	for_each(m_labels.begin(), m_labels.end(), DeletePointerFunctor<const PackLabel>());
	for_each(m_anchors.begin(), m_anchors.end(), DeletePointerFunctor<const PackAnchor>());
}

void LabelBuilder::Traverse(d2d::IVisitor& visitor) const
{
 	for (int i = 0, n = m_labels.size(); i < n; ++i) {
		bool has_next;
		visitor.Visit(const_cast<PackLabel*>(m_labels[i]), has_next);
		if (!has_next) {
			break;
		}
 	}
}

const IPackNode* LabelBuilder::Create(const d2d::FontSprite* spr)
{
	if (IsAnchorNode(spr)) {
		PackAnchor* anchor = new PackAnchor;
		anchor->name = spr->name;
		m_anchors.push_back(anchor);
		return anchor;
	}

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

bool LabelBuilder::IsAnchorNode(const d2d::FontSprite* spr) const
{
	return spr && spr->font.empty() && spr->color == d2d::Colorf(0, 0, 0, 0);
}

}