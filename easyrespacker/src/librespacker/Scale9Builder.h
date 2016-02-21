#ifndef _EASYRESPACKER_SCALE9_BUILDER_H_
#define _EASYRESPACKER_SCALE9_BUILDER_H_

#include "INodeBuilder.h"

#include <map>

namespace escale9 { class Sprite; class Symbol; }

namespace erespacker
{

class IPackNode;
class PackPicture;

class Scale9Builder : public INodeBuilder
{
public:
	Scale9Builder();
	virtual ~Scale9Builder();

	virtual void Traverse(ee::Visitor& visitor) const;

	const IPackNode* Create(const escale9::Sprite* spr);

private:
	const IPackNode* Query(const escale9::Sprite* spr) const;

	void Load(const escale9::Sprite* spr, PackPicture* pic);

private:
	struct Value
	{
		float w, h;
		const IPackNode* node;
	};

private:
	std::multimap<const escale9::Symbol*, Value> m_map_data;

}; // Scale9Builder

}

#endif // _EASYRESPACKER_SCALE9_BUILDER_H_
