#include "RectBinArrange.h"
#include "config.h"
#include "Context.h"

// Rectangle Bin
#include <Rect.h>
#include <GuillotineBinPack.h>
#include <MaxRectsBinPack.h>
#include <ShelfBinPack.h>
#include <SkylineBinPack.h>

using namespace epacker;

void RectBinArrange::arrange(const std::vector<d2d::ImageSprite*>& sprites)
{
// 	std::vector<d2d::ImageSprite*> sorted(sprites);
// 	sortByMaxEdge(sorted);
// 
// 	std::vector<RectSize> input;
// 	BeforePacking(sorted, input);
// 
// 	std::vector<Rect> output;
// 	GuillotineBinPackAlg(input, output);
// // 	MaxRectsBinPackAlg(input, output);
// // 	ShelfBinPackAlg(input, output);
// // 	SkylineBinPackAlg(input, output);
// 	
// 	AfterPacking(sorted, output);

	//////////////////////////////////////////////////////////////////////////

	m_tex_account = 0;

	std::vector<d2d::ImageSprite*> sorted(sprites);
	sortByMaxEdge(sorted);

	int count = 0;

	float x_offset = 0;
	std::vector<d2d::ImageSprite*> remains(sorted);
	while (!remains.empty())
	{
		std::vector<RectSize> input;
		BeforePacking(remains, input);

		std::vector<Rect> output;
		GuillotineBinPackAlg(input, output);
		// 	MaxRectsBinPackAlg(input, output);
		// 	ShelfBinPackAlg(input, output);
		// 	SkylineBinPackAlg(input, output);

		std::vector<d2d::ImageSprite*> _remains;
		AfterPacking(x_offset, remains, output, _remains);
		remains = _remains;

		x_offset += Context::Instance()->width * TEXTURE_X_OFFSET_FACTOR;

		m_tex_account++;

		if (count >= 100) {
			break;
		}
		++count;
	}
}

void RectBinArrange::BeforePacking(const std::vector<d2d::ImageSprite*>& sorted, std::vector<RectSize>& input) const
{
	input.reserve(sorted.size());
	const float s = Context::Instance()->scale,
		p = Context::Instance()->padding;
	for (size_t i = 0, n = sorted.size(); i < n; ++i)
	{
		RectSize rect;
		rect.width = sorted[i]->GetSymbol().GetSize().xLength() * s + p;
		rect.height = sorted[i]->GetSymbol().GetSize().yLength() * s + p;
		input.push_back(rect);
	}
}

void RectBinArrange::AfterPacking(float xoffset,
								  std::vector<d2d::ImageSprite*>& sprites, 
								  const std::vector<Rect>& output,
								  std::vector<d2d::ImageSprite*>& remains) const
{
	const float s = Context::Instance()->scale,
		p = Context::Instance()->padding;

	for (size_t i = 0, n = output.size(); i < n; ++i)
	{
		const Rect& rect = output[i];
		d2d::ImageSprite* sprite = sprites[i];
		if (rect.height != 0)
		{
			d2d::Rect r = sprite->GetSymbol().GetSize();
			d2d::Vector pos;
			float angle = 0;
			if (r.xLength() == rect.width && r.yLength() == rect.height)
			{
				pos.x = rect.x + r.xLength() * 0.5f * s + p - r.xCenter();
				pos.y = rect.y + r.yLength() * 0.5f * s + p - r.yCenter();
			}
			else if (r.xLength() == rect.height && r.yLength() == rect.width)
			{
				angle = d2d::PI * 0.5f;
				// 					pos.x = output.x + r.yLength() * 0.5f * s + p/* - r.yCenter()*/;
				// 					pos.y = output.y + r.xLength() * 0.5f * s + p /*- r.xCenter()*/;

				pos.x = rect.x + 0.5f * rect.width - r.xCenter();
				pos.y = rect.y + 0.5f * rect.height - r.yCenter();
			}
			else
			{
				assert(0);
			}
			pos.x += xoffset;
			sprite->SetTransform(pos, angle);
		}
		else
		{
			remains.push_back(sprite);
		}
	}
}

void RectBinArrange::GuillotineBinPackAlg(const std::vector<RectSize>& input, std::vector<Rect>& output) const
{
	GuillotineBinPack::FreeRectChoiceHeuristic cType = GuillotineBinPack::RectBestAreaFit;
	GuillotineBinPack::GuillotineSplitHeuristic sType = GuillotineBinPack::SplitShorterLeftoverAxis;
	do 
	{
		output.clear();
		output.reserve(input.size());

		GuillotineBinPack bin(Context::Instance()->width, Context::Instance()->height);

		const float s = Context::Instance()->scale,
			p = Context::Instance()->padding;

		bool reinsert = false;
		for (int i = 0, n = input.size(); i < n; ++i)
		{
			Rect rect = bin.Insert(input[i].width, input[i].height,
				true, // Use the rectangle merge heuristic rule
				cType,
				sType);

			output.push_back(rect);
			if (rect.height == 0) {
				reinsert = true;
			}

			if (rect.height == 0) {
				if (cType == GuillotineBinPack::RectWorstLongSideFit && 
					sType == GuillotineBinPack::SplitLongerAxis) {
										
				} else {
					reinsert = true;
				}
			}
		}
		if (!reinsert) {
			break;
		}

		sType = (GuillotineBinPack::GuillotineSplitHeuristic)(sType + 1);
		if (sType > GuillotineBinPack::SplitLongerAxis)
		{
			cType = (GuillotineBinPack::FreeRectChoiceHeuristic)(cType + 1);
			sType = GuillotineBinPack::SplitShorterLeftoverAxis;
		}
	} while (cType <= GuillotineBinPack::RectWorstLongSideFit && sType <= GuillotineBinPack::SplitLongerAxis);
}

void RectBinArrange::MaxRectsBinPackAlg(std::vector<RectSize>& input, std::vector<Rect>& output) const
{
  	MaxRectsBinPack bin(Context::Instance()->width, Context::Instance()->height);
  	bin.Insert(input, output, MaxRectsBinPack::RectBestShortSideFit);
}

void RectBinArrange::ShelfBinPackAlg(const std::vector<RectSize>& input, std::vector<Rect>& output) const
{
  	ShelfBinPack::ShelfChoiceHeuristic type = ShelfBinPack::ShelfFirstFit;
  	do
  	{
  		ShelfBinPack bin(Context::Instance()->width, Context::Instance()->height, false);
  		size_t i, n;
  		for (i = 0, n = input.size(); i < n; ++i)
  		{
  			Rect rect = bin.Insert(input[i].width, input[i].height,
 				ShelfBinPack::ShelfFirstFit);
			output.push_back(rect);
			if (rect.height == 0) {
				break;
			}
  		}  
  		if (i == n)
  			break;
  
  		type = (ShelfBinPack::ShelfChoiceHeuristic)(type + 1);
  
  	} while (type <= ShelfBinPack::ShelfWorstWidthFit);
}

void RectBinArrange::SkylineBinPackAlg(std::vector<RectSize>& input, std::vector<Rect>& output) const
{
  	SkylineBinPack bin(Context::Instance()->width, Context::Instance()->height, true);
  	bin.Insert(input, output, SkylineBinPack::LevelMinWasteFit);
}
