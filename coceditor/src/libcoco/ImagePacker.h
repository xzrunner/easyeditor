#ifndef _LIBCOCO_IMAGE_PACKER_H_
#define _LIBCOCO_IMAGE_PACKER_H_

#include "INodePacker.h"
#include "PackPicture.h"

#include <drag2d.h>

namespace libcoco
{

class IPackNode;

class ImagePacker : public INodePacker
{
public:
	ImagePacker();
	virtual ~ImagePacker();

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const TexturePacker& tp) const;

	const IPackNode* Create(const d2d::ImageSprite* spr);

	static void LoadPictureQuad(const d2d::ImageSprite* img, PackPicture::Quad& quad);
	
	static void TransScreen(PackPicture::Quad& quad, const d2d::ISprite* spr);

private:
	std::vector<IPackNode*> m_nodes;

}; // ImagePacker

}

#endif // _LIBCOCO_IMAGE_PACKER_H_
