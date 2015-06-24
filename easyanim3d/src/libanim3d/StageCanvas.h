#ifndef _EASYANIM3D_STAGE_CANVAS_H_
#define _EASYANIM3D_STAGE_CANVAS_H_

#include <drag2d.h>
#include <easy3d.h>

namespace eanim3d
{

class StageCanvas : public e3d::StageCanvas
{
public:
	StageCanvas(d2d::EditPanel* edit_panel);

protected:
	virtual void onSize(int w, int h);
	virtual void OnDraw();

private:
	StagePanel* m_stage;

}; // StageCanvas

}

#endif // _EASYANIM3D_STAGE_CANVAS_H_