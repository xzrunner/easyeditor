#ifndef _EASYPARTICLE2D_STAGEPANEL_H_
#define _EASYPARTICLE2D_STAGEPANEL_H_

#include <drag2d.h>

namespace eparticle2d
{

class LibraryPanel;
class ParticleSystem;

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		LibraryPanel* library);
	virtual ~StagePanel();

	ParticleSystem* GetParticleSystem() const { return m_ps; }

private:
	ParticleSystem* m_ps;

}; // StagePanel

}

#endif // _EASYPARTICLE2D_STAGEPANEL_H_
