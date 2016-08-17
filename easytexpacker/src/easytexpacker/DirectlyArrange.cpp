#include "DirectlyArrange.h"
#include "Context.h"

#include <ee/ImageSprite.h>

namespace etexpacker
{

void DirectlyArrange::Arrange(const std::vector<ee::ImageSprite*>& sprs)
{
	std::vector<ee::ImageSprite*> sorted(sprs);
	SortByArea(sorted);

	int sx = 0, sy = 0;
	int hMax = 0;
	const float s = Context::Instance()->scale,
		p = Context::Instance()->padding;
	for (size_t i = 0, n = sorted.size(); i < n; ++i)
	{
		ee::ImageSprite* spr = sorted[i];
		const float width = spr->GetSymbol()->GetBounding().Width() * s + p,
			height = spr->GetSymbol()->GetBounding().Height() * s + p;

		if (sx + width <= Context::Instance()->width)
		{
			spr->SetPosition(sm::vec2(sx + width * 0.5f, sy + height * 0.5f));

			sx += width;
			if (height > hMax)
				hMax = height;
		}
		else
		{
			sx = 0;
			sy += hMax;
			hMax = height;
			spr->SetPosition(sm::vec2(sx + width * 0.5f, sy + height * 0.5f));
			sx = width;
		}
	}
}

}
