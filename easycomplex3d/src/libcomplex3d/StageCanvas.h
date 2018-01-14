#ifndef _EASYCOMPLEX3D_STAGE_CANVAS_H_
#define _EASYCOMPLEX3D_STAGE_CANVAS_H_

#include <easynode3d/StageCanvas.h>

namespace ee { class LibraryPanel; }
namespace enode3d { class StagePanel; }

namespace ecomplex3d
{

class StageCanvas : public enode3d::StageCanvas
{
public:
	StageCanvas(enode3d::StagePanel* stage, ee::LibraryPanel* library);

protected:
	virtual void DrawBackground() const override;

}; // StageCanvas

}

#endif // _EASYCOMPLEX3D_STAGE_CANVAS_H_
