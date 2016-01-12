#ifndef _LIBRESPACKER_SHAPE_BUILDER_H_
#define _LIBRESPACKER_SHAPE_BUILDER_H_

#include "INodeBuilder.h"

#include <map>

namespace etexture { class Symbol; }

namespace librespacker
{

class IPackNode;
class PackShape;

class ShapeBuilder : public INodeBuilder
{
public:
	ShapeBuilder();
	virtual ~ShapeBuilder();

	virtual void Traverse(d2d::IVisitor& visitor) const;

	bool CanHandle(const etexture::Symbol* symbol) const;

	const IPackNode* Create(const etexture::Symbol* symbol);

private:
	void Load(const etexture::Symbol* symbol, PackShape* shape);

private:
	std::map<const etexture::Symbol*, const PackShape*> m_map_data;	

}; // ShapeBuilder

}

#endif // _LIBRESPACKER_SHAPE_BUILDER_H_
