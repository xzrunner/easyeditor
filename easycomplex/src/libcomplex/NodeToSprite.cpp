#include "NodeToSprite.h"

#include "PictureToSpr.h"
#include "LabelToSpr.h"
#include "AnimationToSpr.h"
#include "AnchorToSpr.h"
#include "Particle3DToSpr.h"
#include "Particle2DToSpr.h"

namespace ecomplex
{

ee::Sprite* NodeToSprite::Trans(const erespacker::IPackNode* node)
{
	if (const erespacker::PackPicture* pic = dynamic_cast<const erespacker::PackPicture*>(node)) {
		return PictureToSpr::Trans(pic);
	} else if (const erespacker::PackLabel* label = dynamic_cast<const erespacker::PackLabel*>(node)) {
		return LabelToSpr::Trans(label);
	} else if (const erespacker::PackAnimation* anim = dynamic_cast<const erespacker::PackAnimation*>(node)) {
		return AnimationToSpr::Trans(anim);
	} else if (const erespacker::PackAnchor* anchor = dynamic_cast<const erespacker::PackAnchor*>(node)) {
		return AnchorToSpr::Trans(anchor);
	} else if (const erespacker::PackParticle3D* p3d = dynamic_cast<const erespacker::PackParticle3D*>(node)) {
		return Particle3DToSpr::Trans(p3d);
	} else if (const erespacker::PackParticle2D* p2d = dynamic_cast<const erespacker::PackParticle2D*>(node)) {
		return Particle2DToSpr::Trans(p2d);
	} else if (const erespacker::PackClipbox* cb = dynamic_cast<const erespacker::PackClipbox*>(node)) {
		return NULL;
	} else {
		throw ee::Exception("NodeToSprite::Node2Sprite unknown type.");
	}
}

}