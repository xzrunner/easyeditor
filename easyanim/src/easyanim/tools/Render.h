#pragma once
#include <drag2d.h>

namespace eanim
{
	class Sprite;

	class Render
	{
	public:
		static void drawPos(const d2d::Vector& pos, float radius);

		static void drawSprites(const std::vector<Sprite*>& sprites);
		static void drawSpriteCenter(Sprite* sprite);
		static void drawSpriteCenterMesh(Sprite* sprite);
		static void drawSpriteCenterPartSkeleton(Sprite* sprite);
		static void drawSpriteCenterWholeSkeleton(Sprite* sprite);

		static void drawRawPixels(const d2d::RawPixels& pixels);
		static void drawRawPixelsBound(const d2d::RawPixels& pixels);
		static void drawRawPixelsSelected(const d2d::RawPixels& original, const d2d::RawPixels::PixelBuf& selected);
		static void drawRawPixelsSelectedFlag(const d2d::RawPixels& original, const d2d::RawPixels::PixelBuf& selected);

	}; // Render
}

