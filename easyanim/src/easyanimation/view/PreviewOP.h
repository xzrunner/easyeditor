#ifndef _EASYANIM_PREVIEW_OP_H_
#define _EASYANIM_PREVIEW_OP_H_



namespace eanim
{

struct PlaySettings;

class PreviewOP : public ee::ZoomViewOP
{
public:
	PreviewOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
		PlaySettings& settings, ee::PlayControl& control);

	virtual bool OnKeyDown(int keyCode);

private:
	PlaySettings& m_settings;

	ee::PlayControl& m_control;

}; // PreviewOP

}

#endif // _EASYANIM_PREVIEW_OP_H_