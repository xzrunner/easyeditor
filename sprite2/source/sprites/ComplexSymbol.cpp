#include "ComplexSymbol.h"
#include "ComplexSprite.h"
#include "S2_Sprite.h"
#include "BoundingBox.h"
#include "RenderParams.h"
#include "DrawNode.h"
#include "RenderScissor.h"

#include <map>

namespace s2
{

ComplexSymbol::ComplexSymbol()
{
	m_scissor.xmin = m_scissor.ymin = m_scissor.xmax = m_scissor.ymax = 0;
}

ComplexSymbol::~ComplexSymbol()
{
	for_each(m_children.begin(), m_children.end(), cu::RemoveRefFonctor<Sprite>());
}

void ComplexSymbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	bool scissor = m_scissor.xmin != 0 || m_scissor.ymin != 0 || m_scissor.xmax != 0 || m_scissor.ymax != 0;

	RenderParams p = params;
	if (spr) {
		p.mt = spr->GetTransMatrix() * params.mt;
		p.color = spr->Color() * params.color;			
	}

	if (scissor) {
		sm::vec2 min = p.mt * sm::vec2(m_scissor.xmin, m_scissor.ymin),
			     max = p.mt * sm::vec2(m_scissor.xmax, m_scissor.ymax);
		if (min.x > max.x) {
			std::swap(min.x, max.x);
		}
		if (min.y > max.y) {
			std::swap(min.y, max.y);
		}
		RenderScissor::Instance()->Push(min.x, min.y, max.x-min.x, max.y-min.y);
	}

	int action = -1;
	if (spr) {
		action = VI_DOWNCASTING<const ComplexSprite*>(spr)->GetAction();
	}
	const std::vector<Sprite*>& sprs = GetSprs(action);
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		DrawNode::Draw(sprs[i], p);
	}

	if (scissor) {
		RenderScissor::Instance()->Pop();
	}
}

sm::rect ComplexSymbol::GetBounding(const Sprite* spr) const
{
	sm::rect b;
	int action = -1;
	if (spr) {
		action = VI_DOWNCASTING<const ComplexSprite*>(spr)->GetAction();
	}
	const std::vector<Sprite*>& sprs = GetSprs(action);
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		sprs[i]->GetBounding()->CombineTo(b);
	}
	return b;
}

int ComplexSymbol::GetActionIdx(const std::string& name) const
{
	int idx = -1;
	for (int i = 0, n = m_actions.size(); i < n; ++i) {
		if (m_actions[i].name == name) {
			idx = i;
			break;
		}
	}
	return idx;
}

bool ComplexSymbol::Add(Sprite* spr, int idx)
{
	spr->AddReference();
	if (m_children.empty() || 
		idx >= m_children.size() ||
		idx < 0) {
		m_children.push_back(spr);
	} else {
		m_children.insert(m_children.begin() + idx, spr);
	}
	return true;
}

bool ComplexSymbol::Remove(Sprite* spr)
{
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		if (spr == m_children[i]) {
			spr->RemoveReference();
			m_children.erase(m_children.begin() + i);
			return true;
		}
	}
	return false;
}

bool ComplexSymbol::Clear()
{
	if (m_children.empty()) {
		return false;
	}

	for_each(m_children.begin(), m_children.end(), cu::RemoveRefFonctor<Sprite>());
	m_children.clear();

	// todo
// 	m_actions.clear();

	return true;
}

bool ComplexSymbol::ResetOrder(const Sprite* spr, bool up)
{
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		if (m_children[i] != spr) {
			continue;
		}
		if (up && i != n - 1) {
			std::swap(m_children[i], m_children[i + 1]);
			return true;
		} else if (!up && i != 0) {
			std::swap(m_children[i], m_children[i - 1]);
			return true;
		}
	}
	return false;
}

bool ComplexSymbol::ResetOrderMost(const Sprite* spr, bool up)
{
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		if (m_children[i] != spr) {
			continue;
		}
		if (up && i != n - 1) {
			Sprite* tmp = m_children[i];
			for (int j = i + 1; j < n; ++j) {
				m_children[j-1] = m_children[j];
			}
			m_children[n - 1] = tmp;
			return true;
		} else if (!up && i != 0) {
			Sprite* tmp = m_children[i];
			for (int j = i - 1; j >= 0; --j) {
				m_children[j+1] = m_children[j];
			}
			m_children[0] = tmp;
			return true;
		}
		return false;
	}
	return false;
}

bool ComplexSymbol::Sort(std::vector<Sprite*>& sprs)
{
	std::map<int, Sprite*> order_sorted;
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		Sprite* obj = sprs[i];
		for (int j = 0, m = m_children.size(); j < m; ++j) {
			if (obj == m_children[j]) {
				order_sorted.insert(std::make_pair(j, obj));
			}
		}
	}

	if (order_sorted.size() != sprs.size()) {
		return false;
	}

	std::vector<Sprite*> list_dst;
	list_dst.reserve(sprs.size());
	std::map<int, Sprite*>::iterator itr = order_sorted.begin();
	for ( ; itr != order_sorted.end(); ++itr) {
		list_dst.push_back(itr->second);
	}
	sprs = list_dst;
	return true;
}

const std::vector<Sprite*>& ComplexSymbol::GetSprs(int action) const
{
	if (action < 0 || action >= m_actions.size()) {
		return m_children;
	} else {
		return m_actions[action].sprs;
	}
}

}