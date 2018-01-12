#include "StageCanvas.h"

#include <node3/PrimitiveDraw.h>

namespace eanim3d
{

StageCanvas::StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
						 ee::MultiSpritesImpl* sprites_impl)
	: enode3d::StageCanvas(stage_wnd, stage, sprites_impl)
{
}

void StageCanvas::DrawBackground() const
{
	n3::PrimitiveDraw::SetColor(0xff0000ff);
	n3::PrimitiveDraw::Cross(sm::vec3(0, 0, 0), sm::vec3(1, 1, 1));
//	n3::PrimitiveDraw::Grids(sm::vec3(-10, -10, 0), sm::vec3(10, 10, 0), sm::vec3(0.5f, 0.5f, FLT_MAX));
}

}