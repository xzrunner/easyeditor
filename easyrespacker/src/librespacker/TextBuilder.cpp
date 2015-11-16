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
	int w, h;
	spr->GetSize(w, h);

	int align_h, align_v;
	spr->GetAlign(align_h, align_v);

	float space_h, space_v;
	spr->GetSpace(space_h, space_v);

	for (int i = 0, n = m_labels.size(); i < n; ++i) 
	{
		const PackLabel* label = m_labels[i];
		if (label->width		== w &&
			label->height		== h &&

			label->font			== spr->GetFont() &&
			label->font_size	== spr->GetFontSize() &&
			label->font_color	== spr->GetFontColor() &&

			label->edge			== spr->GetEdge() &&
			label->edge_size	== spr->GetEdgeSize() &&
			label->edge_color	== spr->GetEdgeColor() &&

			label->align_hori	== align_h &&
			label->align_vert	== align_v &&

			label->space_hori	== space_h &&
			label->space_vert	== space_v &&

			label->text			== spr->GetText() &&
			label->tid			== spr->GetTID()) {
			return label;
		}
	} 

	PackLabel* node = new PackLabel;

	node->width	= w;
	node->height = h;

	node->font = spr->GetFont();
	node->font_size = spr->GetFontSize();
	node->font_color = spr->GetFontColor();

	node->edge = spr->GetEdge();
	node->edge_size = spr->GetEdgeSize();
	node->edge_color = spr->GetEdgeColor();

	node->align_hori = align_h;
	node->align_vert = align_v;

	node->space_hori = space_h;
	node->space_vert = space_v;

	node->text = spr->GetText();
	node->tid = spr->GetTID();

	m_labels.push_back(node);

	return node;
}

}