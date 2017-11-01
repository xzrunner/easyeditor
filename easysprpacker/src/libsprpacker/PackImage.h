#ifndef _EASYSPRPACKER_PACK_IMAGE_H_
#define _EASYSPRPACKER_PACK_IMAGE_H_

#include "PackNode.h"

#include <ee/Image.h>
#include <ee/ImageSymbol.h>

#include <SM_Vector.h>
#include <sprite2/ImageSymbol.h>

namespace esprpacker
{

class PackImage : public PackNode
{
public:
	PackImage(const std::shared_ptr<ee::ImageSymbol>& sym);
	
	/**
	 *  @interface
	 *    lua
	 */
	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp) const;
	
	/**
	 *  @interface
	 *    bin
	 */
	virtual int SizeOfUnpackFromBin() const;
	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr, const ee::TexturePacker& tp) const;

private:
	void Init(const std::shared_ptr<ee::ImageSymbol>& sym);

	void GetPackRegion(const ee::TexturePacker& tp, sm::i16_rect& quad, sm::ivec2& offset) const;

private:
	ee::ImageConstPtr m_img = nullptr;

}; // PackImage

}

#endif // _EASYSPRPACKER_PACK_IMAGE_H_