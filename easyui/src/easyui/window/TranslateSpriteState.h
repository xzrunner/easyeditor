#ifndef _EASYUI_WINDOW_TRANSLATE_SPRITE_STATE_H_
#define _EASYUI_WINDOW_TRANSLATE_SPRITE_STATE_H_

#include <ee/TranslateSpriteState.h>

namespace eui
{
namespace window
{

class StagePanel;
class AnchorMgr;

class TranslateSpriteState : public ee::TranslateSpriteState
{
public:
	TranslateSpriteState(StagePanel* stage, ee::SpriteSelection* selection, 
		const ee::Vector& first_pos, AnchorMgr* anchor_mgr);

	virtual void OnMouseRelease(const ee::Vector& pos);

private:
	class Visitor : public ee::Visitor
	{
	public:
		Visitor(AnchorMgr* anchor_mgr) : m_anchor_mgr(anchor_mgr) {}
		virtual void Visit(ee::Object* object, bool& next);
	private:
		AnchorMgr* m_anchor_mgr;
	}; // Visitor

private:
	StagePanel* m_stage;

	AnchorMgr* m_anchor_mgr;

}; // TranslateSpriteState

}
}

#endif // _EASYUI_WINDOW_TRANSLATE_SPRITE_STATE_H_