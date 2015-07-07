#ifndef _EASYIMAGE_STAGE_CANVAS_H_
#define _EASYIMAGE_STAGE_CANVAS_H_

#include <drag2d.h>

namespace eimage
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

#endif // _EASYIMAGE_STAGE_CANVAS_H_
