#ifndef _EASYSPRPACKER_LABEL_BUILDER_H_
#define _EASYSPRPACKER_LABEL_BUILDER_H_

#include "NodeBuilder.h"

#include <CU_Singleton.h>

#include <vector>

namespace etext { class Sprite; }

namespace esprpacker
{

class PackLabel;

class LabelBuilder : public NodeBuilder
{
public:
	virtual void Traverse(ee::Visitor<PackNode>& visitor) const;

	virtual void Clear();

	const PackNode* Create(const etext::Sprite* spr);

private:
	std::vector<const PackLabel*> m_data;

	SINGLETON_DECLARATION(LabelBuilder);

}; // LabelBuilder

}

#endif // _EASYSPRPACKER_LABEL_BUILDER_H_