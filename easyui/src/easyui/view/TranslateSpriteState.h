#ifndef _EASYUI_TRANSLATE_SPRITE_STATE_H_
#define _EASYUI_TRANSLATE_SPRITE_STATE_H_

#include <drag2d.h>

namespace eui
{

class AnchorMgr;

class TranslateSpriteState : public d2d::TranslateSpriteState
{
public:
	TranslateSpriteState(d2d::SpriteSelection* selection, const d2d::Vector& first_pos, 
		AnchorMgr* anchor_mgr);

	virtual d2d::AbstractAtomicOP* OnMouseRelease(const d2d::Vector& pos);

private:
	class Visitor : public d2d::IVisitor
	{
	public:
		Visitor(AnchorMgr* anchor_mgr) : m_anchor_mgr(anchor_mgr) {}
		virtual void visit(d2d::Object* object, bool& bFetchNext);
	private:
		AnchorMgr* m_anchor_mgr;
	}; // Visitor

private:
	AnchorMgr* m_anchor_mgr;

}; // TranslateSpriteState

}

#endif // _EASYUI_TRANSLATE_SPRITE_STATE_H_