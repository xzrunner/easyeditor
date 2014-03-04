#ifndef _EASYPARTICLE2D_STAGE_PANEL_H_
#define _EASYPARTICLE2D_STAGE_PANEL_H_

#include <drag2d.h>

#include "ParticleSystem.h"
#include "ToolbarPanel.h"

namespace eparticle2d
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
	love::graphics::opengl::ParticleSystem* m_particle;

}; // StagePanel

}

#endif // _EASYPARTICLE2D_STAGE_PANEL_H_
