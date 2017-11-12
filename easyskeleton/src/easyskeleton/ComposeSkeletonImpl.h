#ifndef _EASYSKELETON_COMPOSE_SKELETON_IMPL_H_
#define _EASYSKELETON_COMPOSE_SKELETON_IMPL_H_

#include <ee/ArrangeSpriteImpl.h>

namespace ee { class PropertySettingPanel; }

namespace eskeleton
{

class StagePanel;
class Joint;

class ComposeSkeletonImpl : public ee::ArrangeSpriteImpl
{
public:
	ComposeSkeletonImpl(StagePanel* stage, ee::PropertySettingPanel* property);

	virtual bool OnKeyDown(int keyCode) override;
	virtual void OnMouseLeftDown(int x, int y) override;
	virtual void OnMouseLeftUp(int x, int y) override;
	virtual void OnMouseRightDown(int x, int y) override;
	virtual void OnMouseDrag(int x, int y) override;

	virtual void OnPopMenuSelected(int type) override;

protected:
	virtual void OnDirectionKeyDown(int type) override;
	virtual void OnSpaceKeyDown() override;

	virtual void SetRightPopupMenu(wxMenu& menu, int x, int y) override;

	virtual ee::ArrangeSpriteState* CreateTranslateState(ee::SpriteSelection* selection, const sm::vec2& first_pos) const override;
	virtual ee::ArrangeSpriteState* CreateRotateState(ee::SpriteSelection* selection, const sm::vec2& first_pos) const override;
	virtual ee::ArrangeSpriteState* CreateScaleState(const ee::SprPtr& spr, const ee::SpriteCtrlNode::Node& ctrl_node) const override;

private:
	Joint* QueryJoint(const sm::vec2& pos) const;

	void AutoAbsorb(const ee::SprPtr& spr) const;

	void UpdateSelectedBody();

private:
	Joint* m_selected_joint;

	sm::vec2 m_first_pos;

}; // ComposeSkeletonImpl

}

#endif // _EASYSKELETON_COMPOSE_SKELETON_IMPL_H_