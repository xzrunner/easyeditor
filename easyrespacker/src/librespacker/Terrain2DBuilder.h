#ifndef _EASYRESPACKER_TERRAIN_BUILDER_H_
#define _EASYRESPACKER_TERRAIN_BUILDER_H_

#include "INodeBuilder.h"

#include <map>
#include <vector>

namespace eterrain2d { class Symbol; }

namespace erespacker
{

class IPackNode;
class PackAnimation;
class PackPicture;

class Terrain2DBuilder : public INodeBuilder
{
public:
	Terrain2DBuilder();
	virtual ~Terrain2DBuilder();

	virtual void Traverse(ee::Visitor<IPackNode>& visitor) const override;

	const IPackNode* Create(const std::shared_ptr<const eterrain2d::Symbol>& sym);

private:
	void Load(const std::shared_ptr<const eterrain2d::Symbol>& sym, PackAnimation* anim);

private:
	std::map<std::shared_ptr<const eterrain2d::Symbol>, const PackAnimation*> m_map_data;

	std::vector<const PackPicture*> m_pics;

}; // Terrain2DBuilder

}

#endif // _EASYRESPACKER_TERRAIN_BUILDER_H_
