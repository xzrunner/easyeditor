#ifndef _EASYP3DINV_STAGE_PANEL_H_
#define _EASYP3DINV_STAGE_PANEL_H_

#include <drag2d.h>

namespace ep3dinv
{

class StageCanvas;
class ParticleSystem;

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, d2d::LibraryPanel* library);
	virtual ~StagePanel();

protected:
	virtual void OnSizeDebug(wxSizeEvent& event);

public:
	ParticleSystem* m_ps;

}; // StagePanel

}

#endif // _EASYP3DINV_STAGE_PANEL_H_