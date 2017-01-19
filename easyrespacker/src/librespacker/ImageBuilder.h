#ifndef _EASYRESPACKER_IMAGE_BUILDER_H_
#define _EASYRESPACKER_IMAGE_BUILDER_H_

#include "INodeBuilder.h"
#include "PackPicture.h"

#include <map>

namespace ee { class ImageSprite; class Sprite; }
namespace s2 { class Symbol; }

namespace erespacker
{

class IPackNode;

class ImageBuilder : public INodeBuilder
{
public:
	ImageBuilder();
	virtual ~ImageBuilder();

	virtual void Traverse(ee::Visitor<IPackNode>& visitor) const;

	const IPackNode* Create(const ee::ImageSprite* spr);

	static void LoadPictureQuad(const ee::ImageSprite* img, PackPicture::Quad& quad);
	
	static void TransScreen(PackPicture::Quad& quad, const ee::Sprite* spr);

private:
	std::map<const s2::Symbol*, std::vector<PackPicture*> > m_nodes;

}; // ImageBuilder

}

#endif // _EASYRESPACKER_IMAGE_BUILDER_H_
