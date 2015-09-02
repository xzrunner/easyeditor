#ifndef _LIBCOCO_ICON_PACKER_H_
#define _LIBCOCO_ICON_PACKER_H_

#include "INodePacker.h"

#include <easyicon.h>

namespace libcoco
{

class IPackNode;
class PackPicture;

class IconPacker : public INodePacker
{
public:
	IconPacker();
	virtual ~IconPacker();

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const TexturePacker& tp) const;

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

}; // IconPacker

}

#endif // _LIBCOCO_ICON_PACKER_H_