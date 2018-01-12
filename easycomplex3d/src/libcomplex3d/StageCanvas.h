#ifndef _EASYCOMPLEX3D_STAGE_CANVAS_H_
#define _EASYCOMPLEX3D_STAGE_CANVAS_H_

#include <easynode3d/StageCanvas.h>

namespace ee { class LibraryPanel; }

namespace ecomplex3d
{

class StageCanvas : public enode3d::StageCanvas
{
public:
	StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
		ee::MultiSpritesImpl* sprites_impl, ee::LibraryPanel* library);

protected:
	virtual void DrawBackground() const override;

}; // StageCanvas

}

#endif // _EASYCOMPLEX3D_STAGE_CANVAS_H_
