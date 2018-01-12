#include "StageCanvas.h"

#include <ee/color_config.h>

#include <node3/PrimitiveDraw.h>

namespace ecomplex3d
{

StageCanvas::StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
						 ee::MultiSpritesImpl* sprites_impl, ee::LibraryPanel* library)
	: enode3d::StageCanvas(stage_wnd, stage, sprites_impl)
{
}

void StageCanvas::DrawBackground() const
{
	n3::PrimitiveDraw::SetColor(ee::LIGHT_RED.ToABGR());
	n3::PrimitiveDraw::Cross(sm::vec3(0, 0, 0), sm::vec3(10, 10, 10));
}

}