#include "SpriteFactory.h"
#include "MorphingSprite.h"
#include "SkeletalSprite.h"

namespace eanim
{

Sprite* SpriteFactory::Instance()->createSprite(SPRITE_TYPE type)
{
	Sprite* sprite = NULL;
	switch (type)
	{
	case e_morphing:
		sprite = new MorphingSprite;
		break;
	case e_skeletal:
		sprite = new SkeletalSprite;
		break;
	}
	return sprite;
}

Sprite* SpriteFactory::Instance()->createMorphingSprite(Symbol* symbol, const ee::Vector& pos, bool isCopyMesh)
{
	return new MorphingSprite(symbol, pos, isCopyMesh);
}

Sprite* SpriteFactory::Instance()->createSkeletalSprite(WholeSkeleton* skeleton)
{
	return new SkeletalSprite(skeleton);
}

} // eanim