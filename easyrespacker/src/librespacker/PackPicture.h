#ifndef _LIBRESPACKER_PACK_PICTURE_H_
#define _LIBRESPACKER_PACK_PICTURE_H_

#include "IPackNode.h"

#include <drag2d.h>

namespace librespacker
{

class PackPicture : public IPackNode
{
public:
	struct Quad
	{
		const d2d::Image* img;

		d2d::Vector texture_coord[4];
		d2d::Vector screen_coord[4];
	};

	std::vector<Quad> quads;

public:
	PackPicture() {}
	PackPicture(int id);

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const d2d::TexturePacker& tp, float scale) const;
	virtual void UnpackFromLua(lua_State* L,
		const std::vector<d2d::Image*>& images);

	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr,
		const d2d::TexturePacker& tp, float scale) const;
	virtual int SizeOfUnpackFromBin() const;
	virtual void UnpackFromBin(uint8_t** ptr, 
		const std::vector<d2d::Image*>& images);

	static void GetImgSrcPos(const d2d::TexturePacker& tp, const d2d::Image* img, 
		const d2d::Vector* texture_coord, int* src);

private:	
	static int SizeOfQuadBin();
	static void QuadToBin(const Quad& quad, uint8_t** ptr, const d2d::TexturePacker& tp);

}; // PackPicture  

}

#endif // _LIBRESPACKER_PACK_PICTURE_H_