#include "EditSprMsg.h"
#include "Socket.h"
#include "MsgHelper.h"
#include "SpriteSelection.h"
#include "FetchAllVisitor.h"

namespace ee
{

void EditSprMsg::Rotate(const SpriteSelection& selection, float rotate)
{
	if (selection.IsEmpty()) {
		return;
	}
	if (rotate == 0) {
		return;
	}
	if (!Socket::Instance()->IsConnected()) {
		return;
	}

	Json::Value val;
	val["op"] = "rotate";
	val["rotate"] = rotate;

	std::vector<Sprite*> children;
	selection.Traverse(FetchAllVisitor<Sprite>(children));
	MsgHelper::FillingSprites(children, val);

	Socket::Instance()->Send(val.toStyledString());
}

void EditSprMsg::Translate(const SpriteSelection& selection, const sm::vec2& offset)
{
	if (selection.IsEmpty()) {
		return;
	}
	if (offset.x == 0 && offset.y == 0) {
		return;
	}
	if (!Socket::Instance()->IsConnected()) {
		return;
	}

 	Json::Value val;

 	val["op"] = "translate";
 	val["offset"]["x"] = offset.x;
 	val["offset"]["y"] = -offset.y;

	std::vector<Sprite*> children;
	selection.Traverse(FetchAllVisitor<Sprite>(children));
	MsgHelper::FillingSprites(children, val);

	Socket::Instance()->Send(val.toStyledString());
}

void EditSprMsg::SetScale(const Sprite* spr, const sm::vec2& pos, const sm::vec2& scale)
{
	if (!spr) {
		return;
	}
	if (!Socket::Instance()->IsConnected()) {
		return;
	}

	Json::Value val;
	val["op"] = "set_scale";
	val["scale"]["x"] = scale.x;
	val["scale"]["y"] = scale.y;
	val["pos"]["x"] = pos.x;
	val["pos"]["y"] = pos.y;

	MsgHelper::FillingSprites(spr, val);

	Socket::Instance()->Send(val.toStyledString());
}

void EditSprMsg::SetColMul(const Sprite* spr, const s2::Color& mul)
{
	if (!spr) {
		return;
	}
	if (!Socket::Instance()->IsConnected()) {
		return;
	}

	Json::Value val;
	val["op"] = "set_color";
	val["mul"] = mul.ToABGR();

	MsgHelper::FillingSprites(spr, val);

	Socket::Instance()->Send(val.toStyledString());
}

void EditSprMsg::SetColAdd(const Sprite* spr, const s2::Color& add)
{
	if (!spr) {
		return;
	}
	if (!Socket::Instance()->IsConnected()) {
		return;
	}

	Json::Value val;
	val["op"] = "set_color";
	val["add"] = add.ToABGR();

	MsgHelper::FillingSprites(spr, val);

	Socket::Instance()->Send(val.toStyledString());
}

}