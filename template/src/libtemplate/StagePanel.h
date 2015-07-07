#ifndef _EASYTEMPLATE_STAGE_PANEL_H_
#define _EASYTEMPLATE_STAGE_PANEL_H_

#include <drag2d.h>

namespace etemplate
{

class StageCanvas;

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::LibraryPanel* library, d2d::PropertySettingPanel* property,
		d2d::ViewPanelMgr* view_panel_mgr);
	virtual ~StagePanel();

	//
	// d2d::EditPanel interface
	//
	virtual void Clear();

protected:
	virtual void OnSizeDebug(wxSizeEvent& event);

private:
	StageCanvas* m_canvas;

}; // StagePanel

}

#endif // _EASYTEMPLATE_STAGE_PANEL_H_