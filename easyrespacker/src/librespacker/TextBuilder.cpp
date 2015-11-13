#include "TextBuilder.h"
#include "PackLabel.h"

#include <easytext.h>

namespace librespacker
{

TextBuilder::TextBuilder()
{
}

TextBuilder::~TextBuilder()
{
	for_each(m_labels.begin(), m_labels.end(), DeletePointerFunctor<const PackLabel>());
}

void TextBuilder::Traverse(d2d::IVisitor& visitor) const
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
	for (int i = 0, n = m_labels.size(); i < n; ++i) 
	{
		const PackLabel* label = m_labels[i];
		int align_h, align_v;
		spr->GetAlign(align_h, align_v);
		int w, h;
		spr->GetSize(w, h);
		if (label->align_hori == align_h &&
			label->align_vert == align_v && 
			label->color == spr->GetFontColor() &&
			label->size == spr->GetFontSize() &&
			label->width == w &&
			label->height == h &&
			label->has_edge == spr->GetEdge()) {
			return label;
		}
	} 

	int align_h, align_v;
	spr->GetAlign(align_h, align_v);
	int w, h;
	spr->GetSize(w, h);

	PackLabel* node = new PackLabel;

	node->align_hori = align_h;
	node->align_vert = align_v;
	
	node->color = spr->GetFontColor();

	node->size = spr->GetFontSize();
	node->width = w;
	node->height = h;

	node->has_edge = spr->GetEdge();

	m_labels.push_back(node);

	return node;
}

}