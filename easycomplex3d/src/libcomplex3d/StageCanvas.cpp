#include "StageCanvas.h"

#include <ee/color_config.h>

#include <node3/PrimitiveDraw.h>

namespace ecomplex3d
{

StageCanvas::StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
						 ee::MultiSpritesImpl* sprites_impl, ee::LibraryPanel* library)
	: enode3d::StageCanvas(stage_wnd, stage, sprites_impl, nullptr, true)
{
}

void StageCanvas::DrawBackground() const
{
	n3::PrimitiveDraw::SetColor(0xff000088);
	n3::PrimitiveDraw::Cross(sm::vec3(0, 0, 0), sm::vec3(100, 100, 100));

	static const int TOT_LEN = 100;
	static const int GRID_EDGE = 5;
	for (int y = -TOT_LEN; y < TOT_LEN; y += GRID_EDGE) {
		for (int x = -TOT_LEN; x < TOT_LEN; x += GRID_EDGE) {
			if ((x + y) % (GRID_EDGE * 2) == 0) {
				n3::PrimitiveDraw::SetColor(0xff444444);
			} else {
				n3::PrimitiveDraw::SetColor(0xff888888);
			}
			n3::PrimitiveDraw::Rect(
				sm::vec3(x, y, 0),
				sm::vec3(x + GRID_EDGE, y + GRID_EDGE, 0));
		}
	}
}

}