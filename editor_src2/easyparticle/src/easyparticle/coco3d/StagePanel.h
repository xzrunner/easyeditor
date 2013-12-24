
#ifndef EPARTICLE_COCO3D_STAGEPANEL_H
#define EPARTICLE_COCO3D_STAGEPANEL_H

#include <drag2d.h>

#include "ParticleSystem.h"

namespace eparticle
{
namespace coco3d
{
	class StagePanel : public d2d::EditPanel
	{
	public:
		StagePanel(wxWindow* parent);
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
}

#endif // EPARTICLE_COCO3D_STAGEPANEL_H
