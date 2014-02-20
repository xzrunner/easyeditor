
#ifndef EPARTICLE_COCO_STAGEPANEL_H
#define EPARTICLE_COCO_STAGEPANEL_H

#include <drag2d.h>

#include "ParticleSystem.h"

namespace eparticle
{
namespace coco
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

	float xRot, yRot;

}; // StagePanel

}
}

#endif // EPARTICLE_COCO_STAGEPANEL_H
