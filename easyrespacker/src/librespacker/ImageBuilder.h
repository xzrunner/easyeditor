#ifndef _LIBRESPACKER_IMAGE_BUILDER_H_
#define _LIBRESPACKER_IMAGE_BUILDER_H_

#include "INodeBuilder.h"
#include "PackPicture.h"



namespace librespacker
{

class IPackNode;

class ImageBuilder : public INodeBuilder
{
public:
	ImageBuilder();
	virtual ~ImageBuilder();

	virtual void Traverse(ee::Visitor& visitor) const;

	const IPackNode* Create(const ee::ImageSprite* spr);

	static void LoadPictureQuad(const ee::ImageSprite* img, PackPicture::Quad& quad);
	
	static void TransScreen(PackPicture::Quad& quad, const ee::Sprite* spr);

private:
	std::vector<IPackNode*> m_nodes;

}; // ImageBuilder

}

#endif // _LIBRESPACKER_IMAGE_BUILDER_H_
