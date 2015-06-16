#ifndef _LR_PREVIEW_CTRL_CAM_OP_H_
#define _LR_PREVIEW_CTRL_CAM_OP_H_

#include <drag2d.h>

namespace lr
{
namespace preview
{

class CtrlCamOP : public d2d::AbstractEditOP
{
public:
	CtrlCamOP(d2d::EditPanel* editPanel);

	virtual bool OnKeyDown(int keyCode);

private:
	void OffsetCamera(float dx, float dy);

}; // CtrlCamOP

}
}

#endif // _LR_PREVIEW_CTRL_CAM_OP_H_