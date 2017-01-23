#ifndef _EASYRESPACKER_IMAGE_BUILDER_H_
#define _EASYRESPACKER_IMAGE_BUILDER_H_

#include "INodeBuilder.h"
#include "PackPicture.h"

#include <map>

namespace ee { class ImageSprite; class Sprite; }
namespace s2 { class Symbol; }

#define IMAGE_BUILDER_CACHE

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

	static void LoadPictureQuad(const ee::ImageSprite* img, PackPicture::Quad& quad, bool force_mat = false);
	
	static void TransScreen(PackPicture::Quad& quad, const ee::Sprite* spr);

private:
#ifdef IMAGE_BUILDER_CACHE
	std::map<const s2::Symbol*, const PackPicture*> m_nodes;
#else
	std::vector<PackPicture*> m_nodes;
#endif // IMAGE_BUILDER_CACHE

}; // ImageBuilder

}

#endif // _EASYRESPACKER_IMAGE_BUILDER_H_
