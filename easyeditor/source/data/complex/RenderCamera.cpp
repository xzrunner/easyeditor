#include "RenderCamera.h"
#include "Pseudo3DCamera.h"
#include "Math2D.h"

namespace ee
{

RenderCamera::RenderCamera()
	: has_height(true)
	, start_height(0)
{
}

void RenderCamera::CalculateZ(const Pseudo3DCamera* cam, 
							 Vector vertices[4], float z[4]) const
{
	if (!has_height) {
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

	float height = (ymax - ymin) * 1.414f;
//	float height = ymax - ymin;
	for (int i = 0; i < 4; ++i) {
		float y = vertices[i].y;
		z[i] = -(y - ymin) / (ymax - ymin) * height * zs/* + start_height*/;
	}
}

float RenderCamera::CalculateDZ(const Pseudo3DCamera* cam, float offy)
{
//	return sin(cam->GetAngle() )
	return 0;
}

}