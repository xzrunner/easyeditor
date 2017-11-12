#ifndef _EASYRESPACKER_SHAPE_BUILDER_H_
#define _EASYRESPACKER_SHAPE_BUILDER_H_

#include "INodeBuilder.h"

#include <map>

namespace etexture { class Symbol; }

namespace erespacker
{

class IPackNode;
class PackShape;

class ShapeBuilder : public INodeBuilder
{
public:
	ShapeBuilder();
	virtual ~ShapeBuilder();

	virtual void Traverse(ee::Visitor<IPackNode>& visitor) const override;

	bool CanHandle(const std::shared_ptr<etexture::Symbol>& sym) const;

	const IPackNode* Create(const std::shared_ptr<etexture::Symbol>& sym);

private:
	void Load(const std::shared_ptr<etexture::Symbol>& sym, PackShape* shape);

private:
	std::map<std::shared_ptr<const etexture::Symbol>, const PackShape*> m_map_data;

}; // ShapeBuilder

}

#endif // _EASYRESPACKER_SHAPE_BUILDER_H_
