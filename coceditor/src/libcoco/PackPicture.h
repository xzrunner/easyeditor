#ifndef _LIBCOCO_PACK_PICTURE_H_
#define _LIBCOCO_PACK_PICTURE_H_

#include "IPackNode.h"

#include <drag2d.h>

namespace escale9 { class Sprite; }

namespace libcoco
{

class PackPicture : public IPackNode
{
public:
	PackPicture(const d2d::ImageSprite* img);
	PackPicture(const escale9::Sprite* scale9);

private:
	void LoadImage(const d2d::ImageSprite* img);
	void LoadScale9(const escale9::Sprite* scale9);

private:
	struct Quad
	{
		const d2d::ImageSymbol* img_symbol;
		
		d2d::Vector texture_coord[4];
		d2d::Vector screen_coord[4];
	};

	static void TransScreen(Quad& quad, const d2d::ISprite* spr);

private:
	std::vector<Quad> m_quads;

}; // PackPicture  

}

#endif // _LIBCOCO_PACK_PICTURE_H_