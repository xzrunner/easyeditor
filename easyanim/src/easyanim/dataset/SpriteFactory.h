#pragma once



namespace eanim
{
	class Sprite;
	class Symbol;
	class WholeSkeleton;

	class SpriteFactory
	{
	public:
		enum SPRITE_TYPE
		{
			e_morphing = 0,
			e_skeletal
		};

	public:
		static Sprite* createSprite(SPRITE_TYPE type);

		static Sprite* createMorphingSprite(Symbol* symbol, const ee::Vector& pos, bool isCopyMesh);
		static Sprite* createSkeletalSprite(WholeSkeleton* skeleton);

	}; // SpriteFactory
}

