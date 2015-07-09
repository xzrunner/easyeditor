#include "BaseStrategy.h"
#include "Context.h"

using namespace epacker;

void BaseStrategy::sortByArea(std::vector<d2d::ImageSprite*>& sprites, bool isDescend/* = true*/) const
{
	std::sort(sprites.begin(), sprites.end(), SpriteCmp(e_area, isDescend));
}

void BaseStrategy::sortByMaxEdge(std::vector<d2d::ImageSprite*>& sprites, bool isDescend/* = true*/) const
{
	std::sort(sprites.begin(), sprites.end(), SpriteCmp(e_maxEdge, isDescend));
}

void BaseStrategy::sortByTotEdges(std::vector<d2d::ImageSprite*>& sprites, bool isDescend/* = true*/) const
{
	std::sort(sprites.begin(), sprites.end(), SpriteCmp(e_totEdges, isDescend));
}

BaseStrategy::SpriteCmp::SpriteCmp(SortStrategy strategy, bool isDescend)
{
	m_strategy = strategy;
	m_isDescend = isDescend;
}

bool BaseStrategy::SpriteCmp::operator() (const d2d::ImageSprite* s0, const d2d::ImageSprite* s1) const
{
	bool isLess;

	switch (m_strategy)
	{
	case e_area:
		isLess = isAreaLess(s0, s1);
		break;
	case e_maxEdge:
		isLess = isEdgeLess(s0, s1);
		break;
	case e_totEdges:
		isLess = isTotEdgesLess(s0, s1);
		break;
	}

	if (m_isDescend) return !isLess;
	else return isLess;
}

bool BaseStrategy::SpriteCmp::isAreaLess(const d2d::ImageSprite* s0, const d2d::ImageSprite* s1) const
{
	const float s = Context::Instance()->scale,
		p = Context::Instance()->padding;

	return (s0->GetSymbol().GetSize().xLength() * s + p) * (s0->GetSymbol().GetSize().yLength() * s + p)
		<= (s1->GetSymbol().GetSize().xLength() * s + p) * (s1->GetSymbol().GetSize().yLength() * s + p);
}

bool BaseStrategy::SpriteCmp::isEdgeLess(const d2d::ImageSprite* s0, const d2d::ImageSprite* s1) const
{
	const float s = Context::Instance()->scale,
		p = Context::Instance()->padding;

	return std::max((s0->GetSymbol().GetSize().xLength() * s + p), (s0->GetSymbol().GetSize().yLength() * s + p))
		<= std::max((s1->GetSymbol().GetSize().xLength() * s + p), (s1->GetSymbol().GetSize().yLength() * s + p));
}

bool BaseStrategy::SpriteCmp::isTotEdgesLess(const d2d::ImageSprite* s0, const d2d::ImageSprite* s1) const
{
	const float s = Context::Instance()->scale,
		p = Context::Instance()->padding;

	return (s0->GetSymbol().GetSize().xLength() * s + p) + (s0->GetSymbol().GetSize().yLength() * s + p)
		<= (s1->GetSymbol().GetSize().xLength() * s + p) + (s1->GetSymbol().GetSize().yLength() * s + p);
}