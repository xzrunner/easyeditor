#include "SpriteCtrlNode.h"
#include "Rect.h"
#include "Sprite.h"
#include "Symbol.h"
#include "Math2D.h"

namespace ee
{

void SpriteCtrlNode::GetSpriteCtrlNodes(const Sprite* sprite, Vector nodes[8])
{
	Rect r = sprite->GetSymbol().GetSize(sprite);
	if (sprite->GetMirrorX()) { 
		r.xmin = -r.xmin;
		r.xmax = -r.xmax;
		std::swap(r.xmin, r.xmax);
	}
	if (sprite->GetMirrorY()) {
		r.ymin = -r.ymin;
		r.ymax = -r.ymax;
		std::swap(r.ymin, r.ymax);
	}

	sm::mat4 t;
	t.SetTransformation(sprite->GetPosition().x, sprite->GetPosition().y, sprite->GetAngle(),
		sprite->GetScale().x, sprite->GetScale().y, 0, 0, sprite->GetShear().x, sprite->GetShear().y);
	// scale
	nodes[0] = Math2D::TransVector(Vector(r.xmin, r.ymax), t);
	nodes[1] = Math2D::TransVector(Vector(r.xmax, r.ymax), t);
	nodes[2] = Math2D::TransVector(Vector(r.xmin, r.ymin), t);
	nodes[3] = Math2D::TransVector(Vector(r.xmax, r.ymin), t);
	// shear
	nodes[4] = Math2D::TransVector(Vector(r.CenterX(), r.ymax), t);
	nodes[5] = Math2D::TransVector(Vector(r.xmin, r.CenterY()), t);
	nodes[6] = Math2D::TransVector(Vector(r.xmax, r.CenterY()), t);
	nodes[7] = Math2D::TransVector(Vector(r.CenterX(), r.ymin), t);
	// fix for offset
	Vector offset = sprite->GetOffset();
	Vector fix = Math2D::RotateVector(-offset, sprite->GetAngle()) + offset;
	for (int i = 0; i < 8; ++i) {
		nodes[i] += fix;
	}
}

void SpriteCtrlNode::GetSpriteCtrlNodesExt(const Sprite* sprite, Vector nodes[4])
{
	Rect r = sprite->GetSymbol().GetSize(sprite);
	if (sprite->GetMirrorX()) { 
		r.xmin = -r.xmin;
		r.xmax = -r.xmax;
		std::swap(r.xmin, r.xmax);
	}
	if (sprite->GetMirrorY()) {
		r.ymin = -r.ymin;
		r.ymax = -r.ymax;
		std::swap(r.ymin, r.ymax);
	}

	sm::mat4 t;
	t.SetTransformation(sprite->GetPosition().x, sprite->GetPosition().y, sprite->GetAngle(),
		sprite->GetScale().x, sprite->GetScale().y, 0, 0, sprite->GetShear().x, sprite->GetShear().y);
	// perspective
	float px = 0, py = 0;
	if (sprite) {
		px = sprite->GetPerspective().x;
		py = sprite->GetPerspective().y;
	}
	nodes[0] = Math2D::TransVector(Vector(r.xmin+px, r.ymax+py), t);
	nodes[1] = Math2D::TransVector(Vector(r.xmax-px, r.ymax-py), t);
	nodes[2] = Math2D::TransVector(Vector(r.xmin-px, r.ymin-py), t);
	nodes[3] = Math2D::TransVector(Vector(r.xmax+px, r.ymin+py), t);
	// fix for offset
	Vector offset = sprite->GetOffset();
	Vector fix = Math2D::RotateVector(-offset, sprite->GetAngle()) + offset;
	for (int i = 0; i < 4; ++i) {
		nodes[i] += fix;
	}
}

}