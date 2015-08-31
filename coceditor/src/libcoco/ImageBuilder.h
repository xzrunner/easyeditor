#ifndef _LIBCOCO_IMAGE_BUILDER_H_
#define _LIBCOCO_IMAGE_BUILDER_H_

#include "INodeBuilder.h"
#include "PackPicture.h"

#include <drag2d.h>

namespace libcoco
{

class IPackNode;

class ImageBuilder : public INodeBuilder
{
public:
	ImageBuilder();
	virtual ~ImageBuilder();

	virtual void ToString(ebuilder::CodeGenerator& gen,
		const TexturePacker& tp) const;

	const IPackNode* Create(const d2d::ImageSprite* spr);

	static void LoadPictureQuad(const d2d::ImageSprite* img, PackPicture::Quad& quad);
	
	static void TransScreen(PackPicture::Quad& quad, const d2d::ISprite* spr);

private:
	std::vector<IPackNode*> m_nodes;

}; // ImageBuilder

}

#endif // _LIBCOCO_IMAGE_BUILDER_H_