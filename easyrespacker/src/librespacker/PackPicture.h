#ifndef _LIBRESPACKER_PACK_PICTURE_H_
#define _LIBRESPACKER_PACK_PICTURE_H_

#include "IPackNode.h"



namespace librespacker
{

class PackPicture : public IPackNode
{
public:
	struct Quad
	{
		const ee::Image* img;

		ee::Vector texture_coord[4];
		ee::Vector screen_coord[4];
	};

	std::vector<Quad> quads;

public:
	PackPicture() {}
	PackPicture(int id);

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp, float scale) const;
	virtual void UnpackFromLua(lua_State* L,
		const std::vector<ee::Image*>& images);

	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr,
		const ee::TexturePacker& tp, float scale) const;
	virtual int SizeOfUnpackFromBin() const;
	virtual void UnpackFromBin(uint8_t** ptr, 
		const std::vector<ee::Image*>& images);

	static void GetImgSrcPos(const ee::TexturePacker& tp, const ee::Image* img, 
		const ee::Vector* texture_coord, int* src);

private:	
	static int SizeOfQuadBin();
	static void QuadToBin(const Quad& quad, uint8_t** ptr, const ee::TexturePacker& tp);

}; // PackPicture  

}

#endif // _LIBRESPACKER_PACK_PICTURE_H_