#include "SpriteTools.h"
#include "dataset/ISprite.h"
#include "common/Settings.h"
#include "render/PrimitiveDraw.h"

namespace d2d
{

void SpriteTools::DrawName(const ISprite* sprite)
{
// 	if (!Settings::bVisibleNodeName) {
// 		return;
// 	}
// 	if (sprite && !sprite->name.empty() && sprite->name[0] != '_') {
// 		PrimitiveDraw::text(sprite->name.c_str());
// 	}
}

void SpriteTools::DrawName(const Screen& scr, const ISprite* sprite, const Matrix& mt)
{
// 	if (!Settings::bVisibleNodeName) {
// 		return;
// 	}
// 	if (sprite && !sprite->name.empty() && sprite->name[0] != '_') {
// 		PrimitiveDraw::text(sprite->name.c_str());
// 	}
}

}