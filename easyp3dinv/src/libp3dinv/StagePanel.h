#ifndef _EASYP3DINV_STAGE_PANEL_H_
#define _EASYP3DINV_STAGE_PANEL_H_



namespace ep3dinv
{

class StageCanvas;
class ParticleSystem;

class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, ee::LibraryPanel* library);
	virtual ~StagePanel();

protected:
	virtual void OnSizeDebug(wxSizeEvent& event);

public:
	ParticleSystem* m_ps;

}; // StagePanel

}

#endif // _EASYP3DINV_STAGE_PANEL_H_