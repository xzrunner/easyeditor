#ifndef _EASYMODELING_PREVIEW_CANVAS_H_
#define _EASYMODELING_PREVIEW_CANVAS_H_

#include <drag2d.h>

namespace emodeling
{
class PreviewPanel;

class PreviewCanvas : public d2d::DynamicStageCanvas
{
public:
	PreviewCanvas(PreviewPanel* editPanel);
	virtual ~PreviewCanvas();

protected:
	virtual void initGL();
	virtual void onDraw();

private:
	static const int FRAME_RATE = 60;

}; // PreviewCanvas

}

#endif // _EASYMODELING_PREVIEW_CANVAS_H_
