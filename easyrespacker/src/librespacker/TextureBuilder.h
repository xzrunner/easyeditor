#ifndef _LIBRESPACKER_ETEXTURE_BUILDER_H_
#define _LIBRESPACKER_ETEXTURE_BUILDER_H_

#include "INodeBuilder.h"

#include <map>

namespace etexture { class Symbol; }

namespace librespacker
{

class IPackNode;
class PackPicture;

class TextureBuilder : public INodeBuilder
{
public:
	TextureBuilder();
	virtual ~TextureBuilder();

	virtual void Traverse(d2d::IVisitor& visitor) const;

	bool CanHandle(const etexture::Symbol* symbol) const;

	const IPackNode* Create(const etexture::Symbol* symbol);
	
private:
	void Load(const etexture::Symbol* symbol, PackPicture* pic);

private:
	std::map<const etexture::Symbol*, const PackPicture*> m_map_data;

}; // TextureBuilder

}

#endif // _LIBRESPACKER_ETEXTURE_BUILDER_H_
