#include "RenderCamera.h"
#include "Pseudo3DCamera.h"
#include "Math2D.h"

#include <assert.h>

namespace ee
{

static const float HEIGHT_VAL = 1.414f;

RenderCamera::RenderCamera()
	: mode(CM_ORTHO)
	, base_y(FLT_MAX)
{
}

void RenderCamera::CalculateZ(const Pseudo3DCamera* cam, 
							  Vector vertices[4], float z[4]) const
{
	if (mode == CM_ORTHO || mode == CM_PERSPECTIVE_NO_HEIGHT) {
		memset(z, 0, sizeof(float) * 4);
		return;
	}

	float zs = sin(cam->GetAngle() * ee::TRANS_DEG_TO_RAD);

	float ymin = FLT_MAX, ymax = -FLT_MAX;
	for (int i = 0; i < 4; ++i) {
		float y = vertices[i].y;
		if (y < ymin) ymin = y;
		if (y > ymax) ymax = y;
	}

//	float zoff = 0;
	if (base_y != FLT_MAX) {
// 		assert(ymin >= base_y);
// 		zoff = (ymin - base_y) * HEIGHT_VAL;

		ymin -= (ymin - base_y);
	}

	float height = (ymax - ymin) * HEIGHT_VAL;
//	float height = ymax - ymin;
	for (int i = 0; i < 4; ++i) {
		float y = vertices[i].y;
		z[i] = -(y - ymin) / (ymax - ymin) * height * zs;
	}
}

}