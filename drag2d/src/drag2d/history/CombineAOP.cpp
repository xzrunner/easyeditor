#include "CombineAOP.h"

namespace d2d
{

void CombineAOP::Insert(AbstractAtomicOP* atomic)
{
	m_atomics.push_back(atomic);
}

void CombineAOP::undo()
{
	for (int i = m_atomics.size() - 1; i >= 0; --i) {
		m_atomics[i]->undo();
	}
}

void CombineAOP::redo()
{
	for (int i = 0, n = m_atomics.size(); i < n; ++i) {
		m_atomics[i]->redo();
	}
}

// todo
Json::Value CombineAOP::store(const std::vector<ISprite*>& sprites)
{
	Json::Value ret;
	return ret;
}

}