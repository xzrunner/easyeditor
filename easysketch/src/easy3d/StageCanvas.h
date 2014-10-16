#ifndef _E3D_STAGE_CANVAS_H_
#define _E3D_STAGE_CANVAS_H_

#include <drag2d.h>

namespace e3d
{

class StageCanvas : public d2d::GLCanvas
{
public:
	StageCanvas(d2d::EditPanel* editPanel);

protected:
	virtual void onSize(int w, int h);

}; // StageCanvas

}

#endif // _E3D_STAGE_CANVAS_H_