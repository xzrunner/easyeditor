#ifndef _LIBCOCO_SCALE9_PACKER_H_
#define _LIBCOCO_SCALE9_PACKER_H_

#include "INodePacker.h"

#include <easyscale9.h>

namespace libcoco
{

class IPackNode;
class PackPicture;

class Scale9Packer : public INodePacker
{
public:
	Scale9Packer();
	virtual ~Scale9Packer();

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

}; // Scale9Packer

}

#endif // _LIBCOCO_SCALE9_PACKER_H_
