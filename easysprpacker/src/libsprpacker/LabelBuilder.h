#ifndef _EASYSPRPACKER_LABEL_BUILDER_H_
#define _EASYSPRPACKER_LABEL_BUILDER_H_

#include "NodeBuilder.h"

#include <cu/cu_macro.h>

#include <json/json.h>

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

	const PackNode* Create(const std::shared_ptr<const etext::Sprite>& spr);

	void OutputExtraInfo(Json::Value& value) const;

private:
	std::vector<const PackLabel*> m_labels;

	CU_SINGLETON_DECLARATION(LabelBuilder);

}; // LabelBuilder

}

#endif // _EASYSPRPACKER_LABEL_BUILDER_H_