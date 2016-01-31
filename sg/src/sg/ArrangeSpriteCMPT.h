#ifndef _SG_ARRANGE_SPRITE_CMPT_H_
#define _SG_ARRANGE_SPRITE_CMPT_H_



namespace sg
{

class StagePanel;

class ArrangeSpriteCMPT : public ee::EditCMPT
{
public:
	ArrangeSpriteCMPT(wxWindow* parent, const wxString& name, 
		ee::PropertySettingPanel* property, StagePanel* stage);
	
protected:
	virtual wxSizer* InitLayout();

private:
	void onUpLevelPress(wxCommandEvent& event);
	void onDownLevelPress(wxCommandEvent& event);

private:
	StagePanel* m_stage_panel;

}; // ArrangeSpriteCMPT 

}

#endif // _SG_ARRANGE_SPRITE_CMPT_H_