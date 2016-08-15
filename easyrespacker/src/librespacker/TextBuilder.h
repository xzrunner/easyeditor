#ifndef _EASYRESPACKER_TEXT_BUILDER_H_
#define _EASYRESPACKER_TEXT_BUILDER_H_

#include "INodeBuilder.h"

#include <json/json.h>

#include <vector>

namespace etext { class Sprite; }

namespace erespacker
{

class IPackNode;
class PackLabel;

class TextBuilder : public INodeBuilder
{
public:
	TextBuilder();
	virtual ~TextBuilder();

	virtual void Traverse(ee::Visitor<IPackNode>& visitor) const;

	const IPackNode* Create(const etext::Sprite* spr);

	void OutputExtraInfo(Json::Value& value) const;

	void CacheBegin();
	void CacheEnd();

private:
	const IPackNode* QueryCache(const etext::Sprite* spr) const;

private:
	std::vector<const PackLabel*> m_labels;

	bool m_cache;
	std::vector<const PackLabel*> m_cache_labels;

}; // TextBuilder

}

#endif // _EASYRESPACKER_TEXT_BUILDER_H_
