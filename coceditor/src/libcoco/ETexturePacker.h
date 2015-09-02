#ifndef _LIBCOCO_ETEXTURE_PACKER_H_
#define _LIBCOCO_ETEXTURE_PACKER_H_

#include "INodePacker.h"

#include <easytexture.h>

namespace libcoco
{

class IPackNode;
class PackPicture;

class ETexturePacker : public INodePacker
{
public:
	ETexturePacker();
	virtual ~ETexturePacker();

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const TexturePacker& tp) const;

	const IPackNode* Create(const etexture::Symbol* symbol);

private:
	void Load(const etexture::Symbol* symbol, PackPicture* pic);

private:
	std::map<const etexture::Symbol*, const PackPicture*> m_map_data;

}; // ETexturePacker

}

#endif // _LIBCOCO_ETEXTURE_PACKER_H_
