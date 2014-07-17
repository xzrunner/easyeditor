#ifndef _EASYIMAGE_PIXEL_DIFF_OP_H_
#define _EASYIMAGE_PIXEL_DIFF_OP_H_

#include <drag2d.h>

namespace eimage
{

class StagePanel;

class PixelDiffOP : public d2d::ZoomViewOP
{
public:
	PixelDiffOP(StagePanel* stage);

	virtual bool onMouseLeftDown(int x, int y);

	virtual bool onDraw() const;
	virtual bool clear();

private:
	StagePanel* m_stage;

//	d2d::ImageSprite *m_left, *m_right;

}; // PixelDiffOP 

}

#endif // _EASYIMAGE_PIXEL_DIFF_OP_H_