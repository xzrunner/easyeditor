#ifndef _LIBCOCO_ICON_BUILDER_H_
#define _LIBCOCO_ICON_BUILDER_H_

#include "INodeBuilder.h"

#include <easyicon.h>

namespace libcoco
{

class IPackNode;
class PackPicture;

class IconBuilder : public INodeBuilder
{
public:
	IconBuilder();
	virtual ~IconBuilder();

	virtual void ToString(ebuilder::CodeGenerator& gen,
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

}; // IconBuilder

}

#endif // _LIBCOCO_ICON_BUILDER_H_