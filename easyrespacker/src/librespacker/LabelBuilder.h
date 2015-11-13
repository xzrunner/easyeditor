#ifndef _LIBRESPACKER_LABEL_BUILDER_H_
#define _LIBRESPACKER_LABEL_BUILDER_H_

#include "INodeBuilder.h"

#include <drag2d.h>

namespace librespacker
{

class IPackNode;
class PackLabel;

class LabelBuilder : public INodeBuilder
{
public:
	LabelBuilder();
	virtual ~LabelBuilder();

	virtual void Traverse(d2d::IVisitor& visitor) const;

	const IPackNode* Create(const d2d::FontSprite* spr);

	void OutputExtraInfo(Json::Value& value) const;

private:
	std::vector<const PackLabel*> m_labels;

}; // LabelBuilder

}

#endif // _LIBRESPACKER_LABEL_BUILDER_H_
