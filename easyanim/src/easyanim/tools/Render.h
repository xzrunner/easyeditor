#pragma once


namespace eanim
{
	class Sprite;

	class Render
	{
	public:
		static void drawPos(const ee::Vector& pos, float radius);

		static void drawSprites(const std::vector<Sprite*>& sprites);
		static void drawSpriteCenter(Sprite* sprite);
		static void drawSpriteCenterMesh(Sprite* sprite);
		static void drawSpriteCenterPartSkeleton(Sprite* sprite);
		static void drawSpriteCenterWholeSkeleton(Sprite* sprite);

		static void drawRawPixels(const ee::RawPixels& pixels);
		static void drawRawPixelsBound(const ee::RawPixels& pixels);
		static void drawRawPixelsSelected(const ee::RawPixels& original, const ee::RawPixels::PixelBuf& selected);
		static void drawRawPixelsSelectedFlag(const ee::RawPixels& original, const ee::RawPixels::PixelBuf& selected);

	}; // Render
}

