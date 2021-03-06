#ifndef _EASYRESPACKER_ETEXTURE_BUILDER_H_
#define _EASYRESPACKER_ETEXTURE_BUILDER_H_

#include "INodeBuilder.h"

#include <map>

namespace etexture { class Symbol; }

namespace erespacker
{

class IPackNode;
class PackPicture;

class TextureBuilder : public INodeBuilder
{
public:
	TextureBuilder();
	virtual ~TextureBuilder();

	virtual void Traverse(ee::Visitor<IPackNode>& visitor) const override;

	bool CanHandle(const std::shared_ptr<etexture::Symbol>& sym) const;

	const IPackNode* Create(const std::shared_ptr<const etexture::Symbol>& sym);
	
private:
	void Load(const std::shared_ptr<const etexture::Symbol>& sym, PackPicture* pic);

private:
	std::map<std::shared_ptr<const etexture::Symbol>, const PackPicture*> m_map_data;

}; // TextureBuilder

}

#endif // _EASYRESPACKER_ETEXTURE_BUILDER_H_
