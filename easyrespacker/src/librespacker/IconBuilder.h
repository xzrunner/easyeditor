#ifndef _EASYRESPACKER_ICON_BUILDER_H_
#define _EASYRESPACKER_ICON_BUILDER_H_

#include "INodeBuilder.h"

#include <map>

namespace eicon { class Sprite; class Symbol; }

namespace erespacker
{

class IPackNode;
class PackPicture;

class IconBuilder : public INodeBuilder
{
public:
	IconBuilder();
	virtual ~IconBuilder();

	virtual void Traverse(ee::Visitor<IPackNode>& visitor) const;

	const IPackNode* Create(const eicon::Sprite* spr);

private:
	const IPackNode* Query(const eicon::Sprite* spr) const;

	void Load(const eicon::Sprite* spr, PackPicture* pic);

private:
	struct Value
	{
		float proc;
		const IPackNode* node;
	};

private:
	std::multimap<const eicon::Symbol*, Value> m_map_data;

}; // IconBuilder

}

#endif // _EASYRESPACKER_ICON_BUILDER_H_