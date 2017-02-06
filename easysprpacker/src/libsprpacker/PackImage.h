#ifndef _EASYSPRPACKER_PACK_IMAGE_H_
#define _EASYSPRPACKER_PACK_IMAGE_H_

#include "PackNode.h"

#include <SM_Vector.h>
#include <sprite2/ImageSymbol.h>

namespace ee { class ImageSymbol; }

namespace esprpacker
{

class PackImage : public PackNode
{
public:
	PackImage(const ee::ImageSymbol* sym);
	virtual ~PackImage();
	
	/**
	 *  @interface
	 *    lua
	 */
	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp, float scale) const;
	
	/**
	 *  @interface
	 *    bin
	 */
	virtual int SizeOfUnpackFromBin() const;
	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, 
		float scale) const;

private:
	void Init(const ee::ImageSymbol* sym);

	void GetPackRegion(const ee::TexturePacker& tp, sm::i16_rect& quad, sm::ivec2& offset) const;

private:
	const ee::Image* m_img;

}; // PackImage

}

#endif // _EASYSPRPACKER_PACK_IMAGE_H_