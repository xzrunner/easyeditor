#include "Tools.h"

namespace libanim
{

void Tools::drawAnimSymbol(const Symbol* symbol, 
						   const d2d::Matrix& mt,
						   int index, 
						   const d2d::Colorf& mul, 
						   const d2d::Colorf& add,
						   const d2d::Colorf& r_trans,
						   const d2d::Colorf& g_trans,
						   const d2d::Colorf& b_trans)
{
	std::vector<d2d::ISprite*> sprites;
	getCurrSprites(symbol, index, sprites);

	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		d2d::SpriteDraw::drawSprite(sprites[i], mt, mul, add, r_trans, g_trans, b_trans);

	for_each(sprites.begin(), sprites.end(), DeletePointerFunctor<d2d::ISprite>());
}

void Tools::getCurrSprites(const Symbol* symbol, int index, std::vector<d2d::ISprite*>& sprites)
{
	for (size_t i = 0, n = symbol->m_layers.size(); i < n; ++i)
	{
		Symbol::Layer* layer = symbol->m_layers[i];

		Symbol::Frame *currFrame = getCurrFrame(layer, index),
			*nextFrame = getNextFrame(layer, index);
		if (!currFrame)
			continue;

		if (!nextFrame)
		{
//			;

			for (size_t i = 0, n = currFrame->sprites.size(); i < n; ++i)
				sprites.push_back(currFrame->sprites[i]->clone());	
		}
		else if (!currFrame->bClassicTween)
		{
			for (size_t i = 0, n = currFrame->sprites.size(); i < n; ++i)
				sprites.push_back(currFrame->sprites[i]->clone());	
		}
		else
		{
			assert(index >= currFrame->index && index < nextFrame->index);
			float process = (float) (index - currFrame->index) / (nextFrame->index - currFrame->index);
			Tools::getTweenSprites(currFrame->sprites, nextFrame->sprites, sprites, process);
		}
	}
}

Symbol::Frame* Tools::getCurrFrame(Symbol::Layer* layer, int index)
{
	Symbol::Frame* ret = NULL;

	for (size_t i = 0, n = layer->frames.size(); i < n; ++i)
	{
		Symbol::Frame* frame = layer->frames[i];
		if (frame->index <= index)
			ret = frame;
		else
			break;
	}
	return ret;
}

Symbol::Frame* Tools::getNextFrame(Symbol::Layer* layer, int index)
{
	for (size_t i = 0, n = layer->frames.size(); i < n; ++i)
	{
		Symbol::Frame* frame = layer->frames[i];
		if (frame->index > index)
			return frame;
	}
	return NULL;
}

void Tools::getTweenSprites(const std::vector<d2d::ISprite*>& start, const std::vector<d2d::ISprite*>& end, 
							std::vector<d2d::ISprite*>& tween, float process)
{
	for (size_t i = 0, n = start.size(); i < n; ++i)
	{
		d2d::ISprite *startSprite = start[i], *endSprite = NULL;
		for (size_t j = 0, m = end.size(); j < m; ++j)
		{
			if (isTweenMatched(*startSprite, *end[j]))
			{
				endSprite = end[j];
				break;
			}
		}

		if (endSprite)
		{
			d2d::ISprite* tweenSprite = startSprite->clone();
			getTweenSprite(startSprite, endSprite, tweenSprite, process);
			tween.push_back(tweenSprite);
		}
		else
		{
			tween.push_back(startSprite->clone());
		}
	}
}

bool Tools::isTweenMatched(d2d::ISprite& s0, d2d::ISprite& s1)
{
	bool xMirror0, yMirror0;
	s0.getMirror(xMirror0, yMirror0);

	bool xMirror1, yMirror1;
	s1.getMirror(xMirror1, yMirror1);

	return &s0.getSymbol() == &s1.getSymbol() && xMirror0 == xMirror1 && yMirror0 == yMirror1;
}

void Tools::getTweenSprite(d2d::ISprite* start, d2d::ISprite* end, d2d::ISprite* tween, float process)
{
	float xscale = (end->getScale().x - start->getScale().x) * process + start->getScale().x,
		yscale = (end->getScale().y - start->getScale().y) * process + start->getScale().y;
	tween->setScale(xscale, yscale);

	d2d::Vector offset = (end->getOffset() - start->getOffset()) * process + start->getOffset();
	tween->setOffset(offset);

	tween->setTransform(d2d::Vector(0, 0), 0);

	d2d::Vector center_s = start->getCenter(),
		center_e = end->getCenter();

	float angle = (end->getAngle() - start->getAngle()) * process + start->getAngle();
	d2d::Vector base_s = start->getPosition() + start->getOffset(),
		base_e = end->getPosition() + end->getOffset();
	d2d::Vector base_t = (base_e - base_s) * process + base_s;
	d2d::Vector pos_t = base_t -  offset;
	tween->setTransform(pos_t, angle);

	float xshear = (end->getShear().x - start->getShear().x) * process + start->getShear().x,
		yshear = (end->getShear().y - start->getShear().y) * process + start->getShear().y;
	tween->setShear(xshear, yshear);

	tween->addCol = cInterpolate(start->addCol, end->addCol, process);
	tween->multiCol = cInterpolate(start->multiCol, end->multiCol, process);

}

} // anim
