#ifndef _D2D_SPRITE_TOOLS_H_
#define _D2D_SPRITE_TOOLS_H_

namespace d2d
{

class ISprite;
class Screen;
class Matrix;
struct LabelStyle;

class SpriteTools
{
public:
	static void DrawName(const Screen& scr, const ISprite* sprite);

	static void DrawName(const Screen& scr, const ISprite* sprite, const Matrix& mt);

private:
	static void InitLabelStyle(LabelStyle& style);

}; // SpriteTools

}

#endif // _D2D_SPRITE_TOOLS_H_
