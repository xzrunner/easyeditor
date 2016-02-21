#ifndef _LR_PREVIEW_CTRL_CAM_OP_H_
#define _LR_PREVIEW_CTRL_CAM_OP_H_

#include <ee/EditOP.h>

namespace lr
{
namespace preview
{

class CtrlCamOP : public ee::EditOP
{
public:
	CtrlCamOP(wxWindow* wnd, ee::EditPanelImpl* stage);

	virtual bool OnKeyDown(int keyCode);

private:
	void OffsetCamera(float dx, float dy);

}; // CtrlCamOP

}
}

#endif // _LR_PREVIEW_CTRL_CAM_OP_H_