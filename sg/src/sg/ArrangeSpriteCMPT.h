#ifndef _SG_ARRANGE_SPRITE_CMPT_H_
#define _SG_ARRANGE_SPRITE_CMPT_H_

#include <drag2d.h>

namespace sg
{

class StagePanel;

class ArrangeSpriteCMPT : public d2d::AbstractEditCMPT
{
public:
	ArrangeSpriteCMPT(wxWindow* parent, const wxString& name, 
		d2d::PropertySettingPanel* property, 
		d2d::ViewPanelMgr* view_panel_mgr,
		StagePanel* stage);
	
protected:
	virtual wxSizer* initLayout();

private:
	void onUpLevelPress(wxCommandEvent& event);
	void onDownLevelPress(wxCommandEvent& event);

private:
	StagePanel* m_stage_panel;

}; // ArrangeSpriteCMPT 

}

#endif // _SG_ARRANGE_SPRITE_CMPT_H_