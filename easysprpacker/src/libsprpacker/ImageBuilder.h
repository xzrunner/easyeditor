#ifndef _EASYSPRPACKER_IMAGE_BUILDER_H_
#define _EASYSPRPACKER_IMAGE_BUILDER_H_

#include "NodeBuilder.h"

#include <CU_Singleton.h>

#include <map>

namespace ee { class ImageSymbol; }

namespace esprpacker
{

class PackNode;
class PackImage;

class ImageBuilder : public NodeBuilder
{
public:
	virtual void Traverse(ee::Visitor<PackNode>& visitor) const;

	virtual void Clear();

	const PackNode* Create(const ee::ImageSymbol* sym);

private:
	std::map<const ee::ImageSymbol*, const PackImage*> m_map_data;

	SINGLETON_DECLARATION(ImageBuilder);

}; // ImageBuilder

}

#endif // _EASYSPRPACKER_IMAGE_BUILDER_H_