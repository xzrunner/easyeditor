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
		StagePanel* stage, d2d::PropertySettingPanel* property);
	
protected:
	virtual wxSizer* initLayout();

private:
	void onUpLevelPress(wxCommandEvent& event);
	void onDownLevelPress(wxCommandEvent& event);

}; // ArrangeSpriteCMPT 

}

#endif // _SG_ARRANGE_SPRITE_CMPT_H_