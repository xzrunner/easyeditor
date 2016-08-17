#include "Math.h"
#include "ChainShape.h"

#include <SM_Calc.h>

#include <assert.h>

namespace eshape
{

void Math::mergeTwoChains(const ChainShape& src0, const ChainShape& src1,
						  std::vector<sm::vec2>& dst)
{
	const std::vector<sm::vec2>& s0 = src0.GetVertices();
	const std::vector<sm::vec2>& s1 = src1.GetVertices();

	dst.clear();
	dst.reserve(s0.size() + s1.size());

	float d[4];
	d[0] = sm::dis_square_pos_to_pos(s0.back(), s1.front());
	d[1] = sm::dis_square_pos_to_pos(s0.back(), s1.back());
	d[2] = sm::dis_square_pos_to_pos(s0.front(), s1.front());
	d[3] = sm::dis_square_pos_to_pos(s0.front(), s1.back());
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

void Math::mergeMultiChains(const std::vector<ChainShape*>& src, std::vector<sm::vec2>& dst)
{
	if (src.size() == 0)
	{
		return;
	}
	else if (src.size() == 1)
	{
		copy(src[0]->GetVertices().begin(), src[0]->GetVertices().end(), back_inserter(dst));
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
		src[i]->AddReference();
		buffer.push_back(src[i]);
	}

	while (buffer.size() > 2)
	{
		int i0, i1;
		findNearestPair(buffer, i0, i1);

		std::vector<sm::vec2> merged;
		mergeTwoChains(*buffer[i0], *buffer[i1], merged);
		
		buffer[i0]->RemoveReference();
		buffer[i1]->RemoveReference();

		buffer.erase(buffer.begin() + i0);
		if (i1 < i0)
			buffer.erase(buffer.begin() + i1);
		else
			buffer.erase(buffer.begin() + i1 - 1);

		buffer.push_back(new ChainShape(merged, false));
	}

	assert(buffer.size() == 2);

	mergeTwoChains(*buffer[0], *buffer[1], dst);
	buffer[0]->RemoveReference();
	buffer[1]->RemoveReference();
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
	const std::vector<sm::vec2>& s0 = chain0.GetVertices();
	const std::vector<sm::vec2>& s1 = chain1.GetVertices();

	float d[4];
	d[0] = sm::dis_square_pos_to_pos(s0.back(), s1.front());
	d[1] = sm::dis_square_pos_to_pos(s0.back(), s1.back());
	d[2] = sm::dis_square_pos_to_pos(s0.front(), s1.front());
	d[3] = sm::dis_square_pos_to_pos(s0.front(), s1.back());
	float nearest = FLT_MAX;
	for (size_t i = 0; i < 4; ++i)
		if (d[i] < nearest)
			nearest = d[i];

	return nearest;
}

}