#ifndef _EASYANIM_STAGE_PANEL_H_
#define _EASYANIM_STAGE_PANEL_H_

#include <drag2d.h>

namespace eanim
{

class Controller;
class KeyFrame;
class SkeletonData;

class StagePanel : public d2d::EditPanel, public d2d::MultiSpritesImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
		d2d::PropertySettingPanel* property,
		d2d::ViewPanelMgr* view_panel_mgr,
		Controller* ctrl);
	virtual ~StagePanel();

	//
	// d2d::EditPanel interface
	//
	virtual void Clear();

	//
	// d2d::MultiSpritesImpl interface
	//
	virtual bool ReorderSprite(d2d::ISprite* sprite, bool up);
	virtual bool InsertSprite(d2d::ISprite* sprite, int idx = -1);
	virtual bool RemoveSprite(d2d::ISprite* sprite);
	virtual bool ClearAllSprite();
	virtual void TraverseSprites(d2d::IVisitor& visitor, 
		d2d::DataTraverseType type = d2d::DT_ALL,
		bool order = true) const;

	SkeletonData& getSkeletonData();

	Controller* GetController() { return m_ctrl; }

private:
	void onMenuAddJointNode(wxCommandEvent& event);
	void onMenuDelJointNode(wxCommandEvent& event);

public:
	enum
	{
		Menu_AddJointNode = 1000,
		Menu_DelJointNode
	};

private:
	Controller* m_ctrl;

	d2d::ViewPanelMgr* m_view_panel_mgr;

}; // StagePanel

}

#endif // _EASYANIM_STAGE_PANEL_H_