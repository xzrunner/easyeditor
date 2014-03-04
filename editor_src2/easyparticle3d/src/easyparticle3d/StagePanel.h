#ifndef _EASYPARTICLE3D_STAGEPANEL_H_
#define _EASYPARTICLE3D_STAGEPANEL_H_

#include <drag2d.h>

#include "ParticleSystem.h"

namespace eparticle3d
{

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame);
	virtual ~StagePanel();

	//
	// d2d::EditPanel interface
	//
	virtual void clear();

public:
	ParticleSystem* m_particle;

	float xRot, yRot;

}; // StagePanel

}

#endif // _EASYPARTICLE3D_STAGEPANEL_H_
