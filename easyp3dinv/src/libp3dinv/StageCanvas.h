#ifndef _EASYP3DINV_STAGE_CANVAS_H_
#define _EASYP3DINV_STAGE_CANVAS_H_

#include <drag2d.h>

namespace ep3dinv
{

class StagePanel;

class StageCanvas : public d2d::OrthoCanvas
{
public:
	StageCanvas(StagePanel* stage);

protected:
	virtual void OnDrawSprites() const;

private:
	StagePanel* m_stage;

}; // StageCanvas

}

#endif // _EASYP3DINV_STAGE_CANVAS_H_