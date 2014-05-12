#ifndef _LIBSHAPE_STAGE_CANVAS_H_
#define _LIBSHAPE_STAGE_CANVAS_H_

#include <drag2d.h>

namespace libshape
{

class StagePanel;

class StageCanvas : public d2d::ShapeStageCanvas
{
public:
	StageCanvas(StagePanel* stage);

public:
	void drawGuideLines();

protected:
	virtual void initGL();
	virtual void onDraw();

	void onMouse(wxMouseEvent& event);
	void onKeyDown(wxKeyEvent& event);

private:
	DECLARE_EVENT_TABLE()

}; // StageCanvas

}

#endif // _LIBSHAPE_STAGE_CANVAS_H_