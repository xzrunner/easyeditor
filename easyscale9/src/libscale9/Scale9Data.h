#ifndef _EASYSCALE9_SCALE9_DATA_H_
#define _EASYSCALE9_SCALE9_DATA_H_

#include <drag2d.h>

namespace escale9
{

enum Scale9Type
{
	e_null = 0,
	e_9Grid,
	e_3GridHor,
	e_3GridVer,
	e_6GridUpper,
	e_9GridHollow
};

class Scale9Data
{
public:
	
	void Resize();

private:
	void ResizeSprite(d2d::ISprite* sprite, const d2d::Vector& center, 
		float width, float height);

private:
	Scale9Type m_type;

	// 2
	// 1
	// 0 1 2
	d2d::ISprite* m_sprites[3][3];

	float m_width, m_height;

}; // Scale9Data

}

#endif // _EASYSCALE9_SCALE9_DATA_H_