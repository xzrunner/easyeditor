#include "GenPentagon.h"

namespace coceditor
{
	void GenPentagon::trigger(float edge, float val[5], d2d::Vector out[5])
	{
		const float a = d2d::PI * 2 / 5;
		float angle = -d2d::PI*0.5 - a * 0.5f;
		for (size_t i = 0; i < 5; ++i, angle += a)
			out[i] = d2d::Math::rotateVector(d2d::Vector(edge, 0.0f), angle) * val[i];
	}
}