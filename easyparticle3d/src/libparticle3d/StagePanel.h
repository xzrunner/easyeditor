#ifndef _EASYPARTICLE3D_STAGEPANEL_H_
#define _EASYPARTICLE3D_STAGEPANEL_H_

#include <drag2d.h>

namespace eparticle3d
{

class LibraryPanel;
class ParticleSystem;

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		LibraryPanel* library);
	virtual ~StagePanel();

	virtual bool Update(int version);

private:
	void UpdateControl();

public:
	ParticleSystem* m_ps;

	float xRot, yRot;

private:
	clock_t m_last_time;

}; // StagePanel

}

#endif // _EASYPARTICLE3D_STAGEPANEL_H_
