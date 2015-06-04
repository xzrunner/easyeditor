#ifndef _DRAG2D_SINGLE_CANVAS_H_
#define _DRAG2D_SINGLE_CANVAS_H_

#include "IStageCanvas.h"

namespace d2d
{

class SingleCanvas : public IStageCanvas
{
public:
	SingleCanvas(EditPanel* stage);

protected:
	virtual void OnDrawWhole() const;

}; // SingleCanvas

}

#endif // _DRAG2D_SINGLE_CANVAS_H_