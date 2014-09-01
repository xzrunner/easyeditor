#include "SpriteCtrlNode.h"

#include "common/Rect.h"
#include "common/Matrix.h"
#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"

namespace d2d
{

void SpriteCtrlNode::GetSpriteCtrlNodes(const ISprite* sprite, Vector nodes[8])
{
	Rect r = sprite->getSymbol().getSize(sprite);
	Matrix t;
	t.setTransformation(sprite->getPosition().x, sprite->getPosition().y, sprite->getAngle(),
		sprite->getScale().x, sprite->getScale().y, 0, 0, sprite->getShear().x, sprite->getShear().y);
	// scale
	nodes[0] = Math::transVector(Vector(r.xMin, r.yMax), t);
	nodes[1] = Math::transVector(Vector(r.xMax, r.yMax), t);
	nodes[2] = Math::transVector(Vector(r.xMin, r.yMin), t);
	nodes[3] = Math::transVector(Vector(r.xMax, r.yMin), t);
	// shear
	nodes[4] = Math::transVector(Vector(r.xCenter(), r.yMax), t);
	nodes[5] = Math::transVector(Vector(r.xMin, r.yCenter()), t);
	nodes[6] = Math::transVector(Vector(r.xMax, r.yCenter()), t);
	nodes[7] = Math::transVector(Vector(r.xCenter(), r.yMin), t);
	// fix for offset
	d2d::Vector offset = sprite->getOffset();
	d2d::Vector fix = Math::rotateVector(-offset, sprite->getAngle()) + offset;
	for (int i = 0; i < 8; ++i)
		nodes[i] += fix;
}

}