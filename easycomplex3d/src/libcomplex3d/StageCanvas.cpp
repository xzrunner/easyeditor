#include "StageCanvas.h"

#include <ee/color_config.h>

#include <painting3/PrimitiveDraw.h>

namespace ecomplex3d
{

StageCanvas::StageCanvas(ee3::StagePanel* stage, ee::LibraryPanel* library)
	: ee3::StageCanvas(stage, nullptr, true)
{
}

void StageCanvas::DrawBackground() const
{
	n3::PrimitiveDraw::SetColor(0xff000088);
	n3::PrimitiveDraw::Cross(sm::vec3(0, 0, 0), sm::vec3(100, 100, 100));

	static const int TOT_LEN = 100;
	static const int GRID_EDGE = 5;
	for (int z = -TOT_LEN; z < TOT_LEN; z += GRID_EDGE) {
		for (int x = -TOT_LEN; x < TOT_LEN; x += GRID_EDGE) {
			if ((x + z) % (GRID_EDGE * 2) == 0) {
				n3::PrimitiveDraw::SetColor(0xff444444);
			} else {
				n3::PrimitiveDraw::SetColor(0xff888888);
			}
			n3::PrimitiveDraw::Rect(
				sm::vec3(x, 0, z),
				sm::vec3(x + GRID_EDGE, 0, z + GRID_EDGE));
		}
	}
}

}