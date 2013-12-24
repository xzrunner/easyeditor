
#ifndef EPARTICLE_STAGE_PANEL_H
#define EPARTICLE_STAGE_PANEL_H

#include <drag2d.h>

#include "coco/ParticleSystem.h"
#include "ToolbarPanel.h"

namespace eparticle
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
		coco::ParticleSystem* m_particle;

	}; // StagePanel
}

#endif // EPARTICLE_STAGE_PANEL_H
