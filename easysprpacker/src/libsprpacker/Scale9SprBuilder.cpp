#include "Scale9SprBuilder.h"
#include "PackScale9Spr.h"

#include <algorithm>

namespace esprpacker
{

SINGLETON_DEFINITION(Scale9SprBuilder);

Scale9SprBuilder::Scale9SprBuilder()
{
}

Scale9SprBuilder::~Scale9SprBuilder()
{
	Clear();
}

void Scale9SprBuilder::Traverse(ee::Visitor<PackNode>& visitor) const
{
	for (int i = 0, n = m_data.size(); i < n; ++i) {
		bool has_next;
		visitor.Visit(const_cast<PackScale9Spr*>(m_data[i]), has_next);
		if (!has_next) {
			break;
		}
	}
}

void Scale9SprBuilder::Clear()
{
	for_each(m_data.begin(), m_data.end(), cu::RemoveRefFonctor<const PackScale9Spr>());
	m_data.clear();
}

const PackNode* Scale9SprBuilder::Create(const escale9::Sprite* spr)
{
	PackScale9Spr* node = new PackScale9Spr(spr);
	m_data.push_back(node);
	node->AddReference();
	return node;
}

}