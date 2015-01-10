#ifndef _EASYTERRAIN3D_STAGE_CANVAS_H_
#define _EASYTERRAIN3D_STAGE_CANVAS_H_

#include <drag2d.h>
#include <easy3d.h>

namespace libterrain3d
{

class IDemo;

class StageCanvas : public e3d::StageCanvas
{
public:
	StageCanvas(d2d::EditPanel* stage);

protected:
	virtual void initGL();
	virtual void onDraw();

private:
	IDemo* m_demo;

}; // StageCanvas

}

#endif // _EASYTERRAIN3D_STAGE_CANVAS_H_