#include "SpriteCtrlNode.h"
#include "Sprite.h"
#include "Symbol.h"
#include "Math2D.h"

namespace ee
{

void SpriteCtrlNode::GetSpriteCtrlNodes(const Sprite* spr, sm::vec2 nodes[8])
{
	sm::rect r = spr->GetSymbol()->GetBounding(spr);
	if (spr->GetMirror().x) { 
		r.xmin = -r.xmin;
		r.xmax = -r.xmax;
		std::swap(r.xmin, r.xmax);
	}
	if (spr->GetMirror().y) {
		r.ymin = -r.ymin;
		r.ymax = -r.ymax;
		std::swap(r.ymin, r.ymax);
	}

	sm::mat4 t;
	t.SetTransformation(spr->GetPosition().x, spr->GetPosition().y, spr->GetAngle(),
		spr->GetScale().x, spr->GetScale().y, 0, 0, spr->GetShear().x, spr->GetShear().y);
	// scale
	nodes[0] = Math2D::TransVector(sm::vec2(r.xmin, r.ymax), t);
	nodes[1] = Math2D::TransVector(sm::vec2(r.xmax, r.ymax), t);
	nodes[2] = Math2D::TransVector(sm::vec2(r.xmin, r.ymin), t);
	nodes[3] = Math2D::TransVector(sm::vec2(r.xmax, r.ymin), t);
	// shear
	sm::vec2 center = r.Center();
	nodes[4] = Math2D::TransVector(sm::vec2(center.x, r.ymax), t);
	nodes[5] = Math2D::TransVector(sm::vec2(r.xmin, center.y), t);
	nodes[6] = Math2D::TransVector(sm::vec2(r.xmax, center.y), t);
	nodes[7] = Math2D::TransVector(sm::vec2(center.x, r.ymin), t);
	// fix for offset
	sm::vec2 offset = spr->GetOffset();
	sm::vec2 fix = Math2D::RotateVector(-offset, spr->GetAngle()) + offset;
	for (int i = 0; i < 8; ++i) {
		nodes[i] += fix;
	}
}

void SpriteCtrlNode::GetSpriteCtrlNodesExt(const Sprite* spr, sm::vec2 nodes[4])
{
	sm::rect r = spr->GetSymbol()->GetBounding(spr);
	if (spr->GetMirror().x) { 
		r.xmin = -r.xmin;
		r.xmax = -r.xmax;
		std::swap(r.xmin, r.xmax);
	}
	if (spr->GetMirror().y) {
		r.ymin = -r.ymin;
		r.ymax = -r.ymax;
		std::swap(r.ymin, r.ymax);
	}

	sm::mat4 t;
	t.SetTransformation(spr->GetPosition().x, spr->GetPosition().y, spr->GetAngle(),
		spr->GetScale().x, spr->GetScale().y, 0, 0, spr->GetShear().x, spr->GetShear().y);
	// perspective
	float px = 0, py = 0;
	if (spr) {
		px = spr->GetPerspective().x;
		py = spr->GetPerspective().y;
	}
	nodes[0] = Math2D::TransVector(sm::vec2(r.xmin+px, r.ymax+py), t);
	nodes[1] = Math2D::TransVector(sm::vec2(r.xmax-px, r.ymax-py), t);
	nodes[2] = Math2D::TransVector(sm::vec2(r.xmin-px, r.ymin-py), t);
	nodes[3] = Math2D::TransVector(sm::vec2(r.xmax+px, r.ymin+py), t);
	// fix for offset
	sm::vec2 offset = spr->GetOffset();
	sm::vec2 fix = Math2D::RotateVector(-offset, spr->GetAngle()) + offset;
	for (int i = 0; i < 4; ++i) {
		nodes[i] += fix;
	}
}

}