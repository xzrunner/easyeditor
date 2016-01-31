#ifndef _EASYANIM_PREVIEW_PANEL_H_
#define _EASYANIM_PREVIEW_PANEL_H_



namespace eanim
{

class PreviewPanel : public ee::EditPanel
{
public:
	PreviewPanel(wxWindow* parent, wxTopLevelWindow* frame,
		ee::PlayControl& ctrl);

	//
	// ee::EditPanel interface
	//
	virtual bool Update(int version);

private:
	ee::PlayControl& m_ctrl;

}; // PreviewPanel

}

#endif // _EASYANIM_PREVIEW_PANEL_H_