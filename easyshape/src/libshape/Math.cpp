#include "Math.h"
#include "ChainShape.h"

namespace libshape
{

void Math::mergeTwoChains(const ChainShape& src0, const ChainShape& src1,
						  std::vector<d2d::Vector>& dst)
{
	const std::vector<d2d::Vector>& s0 = src0.getVertices();
	const std::vector<d2d::Vector>& s1 = src1.getVertices();

	dst.clear();
	dst.reserve(s0.size() + s1.size());

	float d[4];
	d[0] = d2d::Math::getDistanceSquare(s0.back(), s1.front());
	d[1] = d2d::Math::getDistanceSquare(s0.back(), s1.back());
	d[2] = d2d::Math::getDistanceSquare(s0.front(), s1.front());
	d[3] = d2d::Math::getDistanceSquare(s0.front(), s1.back());
	float nearest = FLT_MAX;
	for (size_t i = 0; i < 4; ++i)
		if (d[i] < nearest)
			nearest = d[i];

	if (nearest == d[0])
	{
		copy(s0.begin(), s0.end(), back_inserter(dst));
		copy(s1.begin(), s1.end(), back_inserter(dst));
	}
	else if (nearest == d[1])
	{
		copy(s0.begin(), s0.end(), back_inserter(dst));
		copy(s1.rbegin(), s1.rend(), back_inserter(dst));
	}
	else if (nearest == d[2])
	{
		copy(s0.rbegin(), s0.rend(), back_inserter(dst));
		copy(s1.begin(), s1.end(), back_inserter(dst));
	}
	else if (nearest == d[3])
	{
		copy(s1.begin(), s1.end(), back_inserter(dst));
		copy(s0.begin(), s0.end(), back_inserter(dst));
	}
}

void Math::mergeMultiChains(const std::vector<ChainShape*>& src, std::vector<d2d::Vector>& dst)
{
	if (src.size() == 0)
	{
		return;
	}
	else if (src.size() == 1)
	{
		copy(src[0]->getVertices().begin(), src[0]->getVertices().end(), back_inserter(dst));
		return;
	}
	else if (src.size() == 2)
	{
		mergeTwoChains(*src[0], *src[1], dst);
		return;
	}

	std::vector<ChainShape*> buffer;
	buffer.reserve(src.size());
	for (size_t i = 0, n = src.size(); i < n; ++i)
	{
		src[i]->retain();
		buffer.push_back(src[i]);
	}

	while (buffer.size() > 2)
	{
		int i0, i1;
		findNearestPair(buffer, i0, i1);

		std::vector<d2d::Vector> merged;
		mergeTwoChains(*buffer[i0], *buffer[i1], merged);
		
		buffer[i0]->release();
		buffer[i1]->release();

		buffer.erase(buffer.begin() + i0);
		if (i1 < i0)
			buffer.erase(buffer.begin() + i1);
		else
			buffer.erase(buffer.begin() + i1 - 1);

		buffer.push_back(new ChainShape(merged, false));
	}

	assert(buffer.size() == 2);

	mergeTwoChains(*buffer[0], *buffer[1], dst);
	buffer[0]->release();
	buffer[1]->release();
}

void Math::findNearestPair(const std::vector<ChainShape*>& chains, int& index0, int& index1)
{
	if (chains.size() == 0)
	{
		index0 = index1 = -1;
		return;
	}
	else if (chains.size() == 1)
	{
		index0 = index1 = 0;
		return;
	}
	else if (chains.size() == 2)
	{
		index0 = 0;
		index1 = 1;
		return;
	}

	float nearest = FLT_MAX;
	for (size_t i = 0, n = chains.size() - 1; i < n; ++i)
	{
		for (size_t j = i + 1, m = chains.size(); j < m; ++j)
		{
			const float dis = getDistanceOfChains(*chains[i], *chains[j]);
			if (dis < nearest)
			{
				nearest = dis;
				index0 = i;
				index1 = j;
			}
		}
	}
}

float Math::getDistanceOfChains(const ChainShape& chain0, const ChainShape& chain1)
{
	const std::vector<d2d::Vector>& s0 = chain0.getVertices();
	const std::vector<d2d::Vector>& s1 = chain1.getVertices();

	float d[4];
	d[0] = d2d::Math::getDistanceSquare(s0.back(), s1.front());
	d[1] = d2d::Math::getDistanceSquare(s0.back(), s1.back());
	d[2] = d2d::Math::getDistanceSquare(s0.front(), s1.front());
	d[3] = d2d::Math::getDistanceSquare(s0.front(), s1.back());
	float nearest = FLT_MAX;
	for (size_t i = 0; i < 4; ++i)
		if (d[i] < nearest)
			nearest = d[i];

	return nearest;
}

}