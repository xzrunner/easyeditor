#include "DirectlyArrange.h"
#include "Context.h"

using namespace etexpacker;

void DirectlyArrange::arrange(const std::vector<d2d::ImageSprite*>& sprites)
{
	std::vector<d2d::ImageSprite*> sorted(sprites);
	sortByArea(sorted);

	int sx = 0, sy = 0;
	int hMax = 0;
	const float s = Context::Instance()->scale,
		p = Context::Instance()->padding;
	for (size_t i = 0, n = sorted.size(); i < n; ++i)
	{
		d2d::ImageSprite* sprite = sorted[i];
		const float width = sprite->GetSymbol().GetSize().xLength() * s + p,
			height = sprite->GetSymbol().GetSize().yLength() * s + p;

		if (sx + width <= Context::Instance()->width)
		{
			sprite->SetTransform(d2d::Vector(sx + width * 0.5f, sy + height * 0.5f), sprite->GetAngle());

			sx += width;
			if (height > hMax)
				hMax = height;
		}
		else
		{
			sx = 0;
			sy += hMax;
			hMax = height;
			sprite->SetTransform(d2d::Vector(sx + width * 0.5f, sy + height * 0.5f), sprite->GetAngle());
			sx = width;
		}
	}
}