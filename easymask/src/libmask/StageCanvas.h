#ifndef _EASYMASK_STAGE_CANVAS_H_
#define _EASYMASK_STAGE_CANVAS_H_

#include <ee/CameraCanvas.h>

namespace emask
{

class StagePanel;

class StageCanvas : public ee::CameraCanvas
{
public:
	StageCanvas(StagePanel* stage);

	void SetMaskRender(bool mask_render) { 
		m_mask_render = mask_render; 
	}

protected:
	virtual void OnDrawSprites() const;

private:
	StagePanel* m_stage;

	bool m_mask_render;

}; // StageCanvas

}

#endif // _EASYMASK_STAGE_CANVAS_H_