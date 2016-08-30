#ifndef _EASYSPRPACKER_TEXTURE_BUILDER_H_
#define _EASYSPRPACKER_TEXTURE_BUILDER_H_

#include "NodeBuilder.h"

#include <CU_Singleton.h>

#include <map>

namespace etexture { class Symbol; }

namespace esprpacker
{

class PackTexture;

class TextureBuilder : public NodeBuilder
{
public:
	virtual void Traverse(ee::Visitor<PackNode>& visitor) const;

	virtual void Clear();

	const PackNode* Create(const etexture::Symbol* sym);

private:
	std::map<const etexture::Symbol*, const PackTexture*> m_map_data;

	SINGLETON_DECLARATION(TextureBuilder);

}; // TextureBuilder

}

#endif // _EASYSPRPACKER_TEXTURE_BUILDER_H_