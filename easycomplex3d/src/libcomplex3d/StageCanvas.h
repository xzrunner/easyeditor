#ifndef _EASYCOMPLEX3D_STAGE_CANVAS_H_
#define _EASYCOMPLEX3D_STAGE_CANVAS_H_

#include <ee3/StageCanvas.h>

namespace ee { class LibraryPanel; }
namespace ee3 { class StagePanel; }

namespace ecomplex3d
{

class StageCanvas : public ee3::StageCanvas
{
public:
	StageCanvas(ee3::StagePanel* stage, ee::LibraryPanel* library);

protected:
	virtual void DrawBackground() const override;

}; // StageCanvas

}

#endif // _EASYCOMPLEX3D_STAGE_CANVAS_H_
