#ifndef _LIBCOCO_SCALE9_BUILDER_H_
#define _LIBCOCO_SCALE9_BUILDER_H_

#include "INodeBuilder.h"

#include <easyscale9.h>

namespace libcoco
{

class IPackNode;
class PackPicture;

class Scale9Builder : public INodeBuilder
{
public:
	Scale9Builder();
	virtual ~Scale9Builder();

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const TexturePacker& tp) const;

	const IPackNode* Create(const escale9::Sprite* spr);

private:
	const IPackNode* Query(const escale9::Sprite* spr) const;

	void Load(const escale9::Sprite* spr, PackPicture* pic);

private:
	struct Value
	{
		int w, h;
		const IPackNode* node;
	};

private:
	std::multimap<const escale9::Symbol*, Value> m_map_data;

}; // Scale9Builder

}

#endif // _LIBCOCO_SCALE9_BUILDER_H_
