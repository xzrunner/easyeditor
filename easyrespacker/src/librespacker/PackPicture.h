#ifndef _EASYRESPACKER_PACK_PICTURE_H_
#define _EASYRESPACKER_PACK_PICTURE_H_

#include "IPackNode.h"

#include <ee/Image.h>

#include <SM_Vector.h>

namespace erespacker
{

class PackPicture : public IPackNode
{
public:
	struct Quad
	{
		ee::ImagePtr img;

		sm::vec2 texture_coord[4];
		sm::vec2 screen_coord[4];
	};

	std::vector<Quad> quads;

public:
	PackPicture() {}
	PackPicture(int id);

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp, float scale) const;
	virtual void UnpackFromLua(lua_State* L,
		const std::vector<ee::ImagePtr>& images);

	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr,
		const ee::TexturePacker& tp, float scale) const;
	virtual int SizeOfUnpackFromBin() const;
	virtual void UnpackFromBin(uint8_t** ptr, 
		const std::vector<ee::ImagePtr>& images);

	static void GetImgSrcPos(const ee::TexturePacker& tp, const ee::Image* img, 
		const sm::vec2* texture_coord, int* src);

private:	
	static int SizeOfQuadBin();
	static void QuadToBin(const Quad& quad, uint8_t** ptr, const ee::TexturePacker& tp);

}; // PackPicture  

}

#endif // _EASYRESPACKER_PACK_PICTURE_H_