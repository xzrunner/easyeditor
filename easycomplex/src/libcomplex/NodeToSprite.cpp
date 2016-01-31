#include "NodeToSprite.h"

#include "PictureToSpr.h"
#include "LabelToSpr.h"
#include "AnimationToSpr.h"
#include "AnchorToSpr.h"
#include "Particle3DToSpr.h"
#include "Particle2DToSpr.h"

namespace ecomplex
{

ee::Sprite* NodeToSprite::Trans(const librespacker::IPackNode* node)
{
	if (const librespacker::PackPicture* pic = dynamic_cast<const librespacker::PackPicture*>(node)) {
		return PictureToSpr::Trans(pic);
	} else if (const librespacker::PackLabel* label = dynamic_cast<const librespacker::PackLabel*>(node)) {
		return LabelToSpr::Trans(label);
	} else if (const librespacker::PackAnimation* anim = dynamic_cast<const librespacker::PackAnimation*>(node)) {
		return AnimationToSpr::Trans(anim);
	} else if (const librespacker::PackAnchor* anchor = dynamic_cast<const librespacker::PackAnchor*>(node)) {
		return AnchorToSpr::Trans(anchor);
	} else if (const librespacker::PackParticle3D* p3d = dynamic_cast<const librespacker::PackParticle3D*>(node)) {
		return Particle3DToSpr::Trans(p3d);
	} else if (const librespacker::PackParticle2D* p2d = dynamic_cast<const librespacker::PackParticle2D*>(node)) {
		return Particle2DToSpr::Trans(p2d);
	} else if (const librespacker::PackClipbox* cb = dynamic_cast<const librespacker::PackClipbox*>(node)) {
		return NULL;
	} else {
		throw ee::Exception("NodeToSprite::Node2Sprite unknown type.");
	}
}

}