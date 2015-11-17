#ifndef _LIBRESPACKER_TEXT_BUILDER_H_
#define _LIBRESPACKER_TEXT_BUILDER_H_

#include "INodeBuilder.h"

#include <vector>
#include <json/json.h>

namespace etext { class Sprite; }

namespace librespacker
{

class IPackNode;
class PackLabel;

class TextBuilder : public INodeBuilder
{
public:
	TextBuilder();
	virtual ~TextBuilder();

	virtual void Traverse(d2d::IVisitor& visitor) const;

	const IPackNode* Create(const etext::Sprite* spr);

	void OutputExtraInfo(Json::Value& value) const;

private:
	std::vector<const PackLabel*> m_labels;

}; // TextBuilder

}

#endif // _LIBRESPACKER_TEXT_BUILDER_H_
