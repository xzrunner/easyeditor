#ifndef EPACKER_STAGE_CANVAS_H
#define EPACKER_STAGE_CANVAS_H

#include <drag2d.h>

namespace epacker
{

class StagePanel;

class StageCanvas : public d2d::DynamicStageCanvas
{
public:
	StageCanvas(StagePanel* stage);

protected:
	virtual void OnDrawSprites() const;

private:
	void DrawRegion() const;

private:
	d2d::MultiSpritesImpl* m_stage_impl;

}; // StageCanvas

}

#endif // EPACKER_STAGE_CANVAS_H