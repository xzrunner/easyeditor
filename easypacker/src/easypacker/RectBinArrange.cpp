#include "RectBinArrange.h"

// Rectangle Bin
#include "Rect.h"
#include "GuillotineBinPack.h"
#include "MaxRectsBinPack.h"
#include "ShelfBinPack.h"
#include "SkylineBinPack.h"
#include "Context.h"

using namespace epacker;

void RectBinArrange::arrange(const std::vector<d2d::ImageSprite*>& sprites)
{
	std::vector<RectSize> input;
	input.reserve(sprites.size());
	const float s = Context::Instance()->scale,
		p = Context::Instance()->padding;
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		RectSize rect;
		rect.width = sprites[i]->getSymbol().getSize().xLength() * s + p;
		rect.height = sprites[i]->getSymbol().getSize().yLength() * s + p;
		input.push_back(rect);
	}

	// GuillotineBinPack
	GuillotineBinPack::FreeRectChoiceHeuristic cType = GuillotineBinPack::RectBestAreaFit;
	GuillotineBinPack::GuillotineSplitHeuristic sType = GuillotineBinPack::SplitShorterLeftoverAxis;
	do 
	{
		GuillotineBinPack bin(Context::Instance()->width, Context::Instance()->height);

		const float s = Context::Instance()->scale,
			p = Context::Instance()->padding;

		size_t i, n;
		for (i = 0, n = input.size(); i < n; ++i)
		{
			Rect output = bin.Insert(input[i].width, input[i].height,
				true, // Use the rectangle merge heuristic rule
				GuillotineBinPack::RectBestShortSideFit,
				GuillotineBinPack::SplitMinimizeArea);
			if (output.height != 0)
			{
				d2d::ISprite* sprite = sprites[i];
				sprite->setTransform(d2d::Vector(output.x+sprite->getSymbol().getSize().xLength()*0.5f*s+p, output.y+sprite->getSymbol().getSize().yLength()*0.5f*s+p), sprite->getAngle());
			}
			else
			{
				break;
			}
		}
		if (i == n)
			break;

		sType = (GuillotineBinPack::GuillotineSplitHeuristic)(sType + 1);
		if (sType > GuillotineBinPack::SplitLongerAxis)
		{
			cType = (GuillotineBinPack::FreeRectChoiceHeuristic)(cType + 1);
			sType = GuillotineBinPack::SplitShorterLeftoverAxis;
		}
	} while (cType <= GuillotineBinPack::RectWorstLongSideFit && sType <= GuillotineBinPack::SplitLongerAxis);

 	////	MaxRectsBinPack
 	//MaxRectsBinPack bin(totWidth, totHeight);
 	//std::vector<Rect> output;
 	//bin.Insert(input, output, MaxRectsBinPack::RectBestShortSideFit);
 	//for (size_t i = 0, n = output.size(); i < n; ++i)
 	//{
 	//	if (output[i].height != 0)
 	//	{
 	//		d2d::ISprite* sprite = sprites[i];
 	//		sprite->setTransform(d2d::Vector(output[i].x+sprite->getSymbol().getSize().xLength()*0.5f, output[i].y+sprite->getSymbol().getSize().yLength()*0.5f), sprite->getAngle());
 	//	}
 	//}

// 	// ShelfBinPack
// 	ShelfBinPack::ShelfChoiceHeuristic type = ShelfBinPack::ShelfFirstFit;
// 	do
// 	{
// 		ShelfBinPack bin(totWidth, totHeight, false);
// 		size_t i, n;
// 		for (i = 0, n = input.size(); i < n; ++i)
// 		{
// 			Rect output = bin.Insert(input[i].width, input[i].height,
//				ShelfBinPack::ShelfFirstFit);
// 			if (output.height != 0)
// 			{
// 				d2d::ISprite* sprite = sprites[i];
// 				sprite->setTransform(d2d::Vector(output.x+sprite->getSymbol().getSize().xLength()*0.5f, output.y+sprite->getSymbol().getSize().yLength()*0.5f), sprite->getAngle());
// 			}
// 			else
// 			{
// 				break;
// 			}
// 		}
// 
// 		if (i == n)
// 			break;
// 
// 		type = (ShelfBinPack::ShelfChoiceHeuristic)(type + 1);
// 
// 	} while (type <= ShelfBinPack::ShelfWorstWidthFit);

// 	// SkylineBinPack
// 	SkylineBinPack bin(totWidth, totHeight, true);
// 	std::vector<Rect> output;
// 	bin.Insert(input, output, SkylineBinPack::LevelMinWasteFit);
// 	for (size_t i = 0, n = output.size(); i < n; ++i)
// 	{
// 		if (output[i].height != 0)
// 		{
// 			d2d::ISprite* sprite = sprites[i];
// 			sprite->setTransform(d2d::Vector(output[i].x+sprite->getSymbol().getSize().xLength()*0.5f, output[i].y+sprite->getSymbol().getSize().yLength()*0.5f), sprite->getAngle());
// 		}
// 	}
}