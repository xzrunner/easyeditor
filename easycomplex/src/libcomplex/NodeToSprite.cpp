#include "NodeToSprite.h"

#include "PictureToSpr.h"
#include "LabelToSpr.h"
#include "AnimationToSpr.h"
#include "AnchorToSpr.h"
#include "Particle3DToSpr.h"
#include "Particle2DToSpr.h"

namespace ecomplex
{

d2d::ISprite* NodeToSprite::Trans(const libcoco::IPackNode* node)
{
	if (const libcoco::PackPicture* pic = dynamic_cast<const libcoco::PackPicture*>(node)) {
		return PictureToSpr::Trans(pic);
	} else if (const libcoco::PackLabel* label = dynamic_cast<const libcoco::PackLabel*>(node)) {
		return LabelToSpr::Trans(label);
	} else if (const libcoco::PackAnimation* anim = dynamic_cast<const libcoco::PackAnimation*>(node)) {
		return AnimationToSpr::Trans(anim);
	} else if (const libcoco::PackAnchor* anchor = dynamic_cast<const libcoco::PackAnchor*>(node)) {
		return AnchorToSpr::Trans(anchor);
	} else if (const libcoco::PackParticle3D* p3d = dynamic_cast<const libcoco::PackParticle3D*>(node)) {
		return Particle3DToSpr::Trans(p3d);
	} else if (const libcoco::PackParticle2D* p2d = dynamic_cast<const libcoco::PackParticle2D*>(node)) {
		return Particle2DToSpr::Trans(p2d);
	} else {
		throw d2d::Exception("NodeToSprite::Node2Sprite unknown type.");
	}
}

}