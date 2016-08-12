#include "ComplexSymbol.h"
#include "S2_Sprite.h"

#include <map>

namespace s2
{

ComplexSymbol::ComplexSymbol()
{
}

void ComplexSymbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	
}

sm::rect ComplexSymbol::GetBounding(const Sprite* spr) const
{
	sm::rect b;
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		m_children[i]->GetBounding()->CombineTo(b);
	}
	return b;
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

	for (int i = 0, n = m_children.size(); i < n; ++i) {
		m_children[i]->RemoveReference();
	}
	m_children.clear();
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

bool ComplexSymbol::Sort(std::vector<Sprite*>& sprites)
{
	std::map<int, Sprite*> order_sorted;
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		Sprite* obj = sprites[i];
		for (int j = 0, m = m_children.size(); j < m; ++j) {
			if (obj == m_children[j]) {
				order_sorted.insert(std::make_pair(j, obj));
			}
		}
	}

	if (order_sorted.size() != sprites.size()) {
		return false;
	}

	std::vector<Sprite*> list_dst;
	list_dst.reserve(sprites.size());
	std::map<int, Sprite*>::iterator itr = order_sorted.begin();
	for ( ; itr != order_sorted.end(); ++itr) {
		list_dst.push_back(itr->second);
	}
	sprites = list_dst;
	return true;
}

}