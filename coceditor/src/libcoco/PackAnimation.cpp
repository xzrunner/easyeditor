#include "PackAnimation.h"

namespace libcoco
{

int PackAnimation::AddComponent(const IPackNode* node)
{
	for (int i = 0, n = m_components.size(); i < n; ++i) {
		if (m_components[i] == node) {
			return i;
		}
	}
	m_components.push_back(node);
	return m_components.size() - 1;
}

}