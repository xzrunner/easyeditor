#ifndef _LIBRESPACKER_IMAGE_BUILDER_H_
#define _LIBRESPACKER_IMAGE_BUILDER_H_

#include "INodeBuilder.h"
#include "PackPicture.h"

#include <drag2d.h>

namespace librespacker
{

class IPackNode;

class ImageBuilder : public INodeBuilder
{
public:
	ImageBuilder();
	virtual ~ImageBuilder();

	virtual void Traverse(d2d::Visitor& visitor) const;

	const IPackNode* Create(const d2d::ImageSprite* spr);

	static void LoadPictureQuad(const d2d::ImageSprite* img, PackPicture::Quad& quad);
	
	static void TransScreen(PackPicture::Quad& quad, const d2d::Sprite* spr);

private:
	std::vector<IPackNode*> m_nodes;

}; // ImageBuilder

}

#endif // _LIBRESPACKER_IMAGE_BUILDER_H_
