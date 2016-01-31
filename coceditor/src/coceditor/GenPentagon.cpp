#include "GenPentagon.h"

namespace coceditor
{
	void GenPentagon::trigger(float edge, float val[5], ee::Vector out[5])
	{
		const float a = ee::PI * 2 / 5;
		float angle = -ee::PI*0.5 - a * 0.5f;
		for (size_t i = 0; i < 5; ++i, angle += a)
			out[i] = ee::Math::rotateVector(ee::Vector(edge, 0.0f), angle) * val[i];
	}
}