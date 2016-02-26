#ifndef _EASYMESH_STAGE_CANVAS_H_
#define _EASYMESH_STAGE_CANVAS_H_

#include <ee/OrthoCanvas.h>

namespace emesh
{

class StagePanel;

class StageCanvas : public ee::OrthoCanvas
{
public:
	StageCanvas(StagePanel* panel, wxGLContext* glctx = NULL);

protected:
	virtual void OnDrawSprites() const;

private:
	StagePanel* m_panel;

}; // StageCanvas

}

#endif // _EASYMESH_STAGE_CANVAS_H_