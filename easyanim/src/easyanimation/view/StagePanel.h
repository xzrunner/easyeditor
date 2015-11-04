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
		Controller* ctrl);
	virtual ~StagePanel();

	//
	// d2d::EditPanel interface
	//
	virtual void Clear();

	//
	//	interface Observer
	//
	virtual void Notify(int sj_id, void* ud);

	//
	// d2d::MultiSpritesImpl interface
	//
	virtual void TraverseSprites(d2d::IVisitor& visitor, 
		d2d::DataTraverseType type = d2d::DT_ALL,
		bool order = true) const;

	SkeletonData& getSkeletonData();

	Controller* GetController() { return m_ctrl; }

private:
	void onMenuAddJointNode(wxCommandEvent& event);
	void onMenuDelJointNode(wxCommandEvent& event);

	void Reorder(d2d::ISprite* spr, bool up);
	void ReorderMost(d2d::ISprite* spr, bool up);
	void Insert(d2d::ISprite* spr);
	void Remove(d2d::ISprite* spr);

public:
	enum
	{
		Menu_AddJointNode = 1000,
		Menu_DelJointNode
	};

private:
	Controller* m_ctrl;

}; // StagePanel

}

#endif // _EASYANIM_STAGE_PANEL_H_