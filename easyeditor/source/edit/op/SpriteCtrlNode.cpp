#include "SpriteCtrlNode.h"
#include "Sprite.h"
#include "Symbol.h"

#include <SM_Calc.h>

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
	nodes[0] = t * sm::vec2(r.xmin, r.ymax);
	nodes[1] = t * sm::vec2(r.xmax, r.ymax);
	nodes[2] = t * sm::vec2(r.xmin, r.ymin);
	nodes[3] = t * sm::vec2(r.xmax, r.ymin);
	// shear
	sm::vec2 center = r.Center();
	nodes[4] = t * sm::vec2(center.x, r.ymax);
	nodes[5] = t * sm::vec2(r.xmin, center.y);
	nodes[6] = t * sm::vec2(r.xmax, center.y);
	nodes[7] = t * sm::vec2(center.x, r.ymin);
	// fix for offset
	sm::vec2 offset = spr->GetOffset();
	sm::vec2 fix = sm::rotate_vector(-offset, spr->GetAngle()) + offset;
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
	nodes[0] = t * sm::vec2(r.xmin+px, r.ymax+py);
	nodes[1] = t * sm::vec2(r.xmax-px, r.ymax-py);
	nodes[2] = t * sm::vec2(r.xmin-px, r.ymin-py);
	nodes[3] = t * sm::vec2(r.xmax+px, r.ymin+py);
	// fix for offset
	sm::vec2 offset = spr->GetOffset();
	sm::vec2 fix = sm::rotate_vector(-offset, spr->GetAngle()) + offset;
	for (int i = 0; i < 4; ++i) {
		nodes[i] += fix;
	}
}

}