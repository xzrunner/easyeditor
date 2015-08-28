#ifndef _LIBCOCO_PACK_PICTURE_H_
#define _LIBCOCO_PACK_PICTURE_H_

#include "IPackNode.h"

#include <drag2d.h>

namespace libcoco
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
	virtual std::string ToString() const;

}; // PackPicture  

}

#endif // _LIBCOCO_PACK_PICTURE_H_