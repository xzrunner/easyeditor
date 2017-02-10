#include "MeshTransform.h"

namespace pm
{

void MeshTransform::Lerp(const MeshTransform& begin, const MeshTransform& end, float process)
{
	Clear();

	int b_ptr = 0, e_ptr = 0;
	int bn = begin.m_trans.size(), en = end.m_trans.size();
	while (b_ptr < bn && e_ptr < en)
	{
		int b_idx = begin.m_trans[b_ptr].first;
		int e_idx = end.m_trans[e_ptr].first;
		const sm::vec2& b_pos = begin.m_trans[b_ptr].second;
		const sm::vec2& e_pos = end.m_trans[e_ptr].second;
		if (b_idx == e_idx) 
		{
			sm::vec2 pos = b_pos + (e_pos - b_pos) * process;
			m_trans.push_back(std::make_pair(b_idx, pos));
			++b_idx;
			++e_idx;
			++b_ptr;
			++e_ptr;
		} 
		else if (b_idx < e_idx) 
		{
			m_trans.push_back(std::make_pair(b_idx, b_pos * (1 - process)));
			++b_idx;
			++b_ptr;
		} 
		else 
		{
			m_trans.push_back(std::make_pair(e_idx, e_pos * process));
			++e_idx;
			++e_ptr;
		}
	}

	if (b_ptr != bn) 
	{
		for (int i = b_ptr; i < bn; ++i) {
			m_trans.push_back(std::make_pair(begin.m_trans[i].first, 
				begin.m_trans[i].second * (1 - process)));
		}
	} 
	else if (e_ptr != en) 
	{
		for (int i = e_ptr; i < en; ++i) {
			m_trans.push_back(std::make_pair(end.m_trans[i].first, 
				end.m_trans[i].second * process));
		}
	}
}

}