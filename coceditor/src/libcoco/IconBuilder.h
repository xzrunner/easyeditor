#ifndef _LIBCOCO_ICON_BUILDER_H_
#define _LIBCOCO_ICON_BUILDER_H_

#include <easyicon.h>

namespace libcoco
{

class IPackNode;
class PackAnimation;

class IconBuilder
{
public:
	IconBuilder();
	~IconBuilder();

	const IPackNode* Create(const eicon::Sprite* spr);

private:
	const IPackNode* Query(const eicon::Sprite* spr) const;

	void Load(const eicon::Sprite* spr, PackAnimation* anim);

private:
	struct Value
	{
		float proc;
		const IPackNode* node;
	};

private:
	std::map<const eicon::Symbol*, Value> m_map_data;

}; // IconBuilder

}

#endif // _LIBCOCO_ICON_BUILDER_H_