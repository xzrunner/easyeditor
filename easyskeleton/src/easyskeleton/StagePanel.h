#ifndef _EASYSKELETON_STAGE_PANEL_H_
#define _EASYSKELETON_STAGE_PANEL_H_

#include <ee/EditPanel.h>
#include <ee/SpritesPanelImpl.h>

namespace ee { class LibraryPanel; class PropertySettingPanel; }

namespace eskeleton
{

class StageCanvas;

class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		ee::LibraryPanel* library, ee::PropertySettingPanel* property);
	virtual ~StagePanel();

public:
	enum
	{
		Menu_AddJointNode = 1000,
		Menu_DelJointNode
	};

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void OnMenuAddJointNode(wxCommandEvent& event);
	void OnMenuDelJointNode(wxCommandEvent& event);

}; // StagePanel

}

#endif // _EASYSKELETON_STAGE_PANEL_H_
