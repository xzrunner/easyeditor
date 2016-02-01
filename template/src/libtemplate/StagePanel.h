#ifndef _EASYTEMPLATE_STAGE_PANEL_H_
#define _EASYTEMPLATE_STAGE_PANEL_H_

#include <ee/EditPanel.h>
#include <ee/SpritesPanelImpl.h>

namespace ee { class LibraryPanel; class PropertySettingPanel; }

namespace etemplate
{

class StageCanvas;

class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		ee::LibraryPanel* library, ee::PropertySettingPanel* property);
	virtual ~StagePanel();

}; // StagePanel

}

#endif // _EASYTEMPLATE_STAGE_PANEL_H_