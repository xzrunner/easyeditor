#include "MaskSymbol.h"
#include "RenderParams.h"
#include "S2_Sprite.h"
#include "DrawMask.h"
#include "DrawNode.h"

namespace s2
{

MaskSymbol::MaskSymbol()
	: m_base(NULL)
	, m_mask(NULL)
{
}

MaskSymbol::~MaskSymbol()
{
	if (m_base) {
		m_base->RemoveReference();
	}
	if (m_mask) {
		m_mask->RemoveReference();
	}
}

void MaskSymbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	RenderParams p = params;
	if (spr) {
		p.mt = spr->GetTransMatrix() * params.mt;
		p.color = spr->Color() * params.color;
	}
	if (m_base && m_mask) {
		DrawMask::Draw(m_base, m_mask, p);
	} else {
		if (m_base) {
			DrawNode::Draw(m_base, p);
		} else if (m_mask) {
			DrawNode::Draw(m_mask, p);
		}
	}
}

sm::rect MaskSymbol::GetBounding(const Sprite* spr) const
{
	if (m_mask) {
		return m_mask->GetBounding();
	} else {
		return sm::rect(sm::vec2(0, 0), 100, 100);
	}
}

void MaskSymbol::SetSymbol(const Symbol* sym, bool is_base)
{
	cu::RefCountObjAssign(is_base ? m_base: m_mask, sym);
}

}