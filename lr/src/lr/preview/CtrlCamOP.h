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
	CtrlCamOP();

	virtual bool onKeyDown(int keyCode);

}; // CtrlCamOP

}
}

#endif // _LR_PREVIEW_CTRL_CAM_OP_H_