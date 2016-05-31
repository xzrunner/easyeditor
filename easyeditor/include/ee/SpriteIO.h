#ifndef _EASYEDITOR_SPRITE_IO_H_
#define _EASYEDITOR_SPRITE_IO_H_

#include <json/json.h>

namespace s2 { class RenderColor; class RenderShader; class RenderCamera; }

namespace ee
{

class Sprite;

class SpriteIO
{
public:
	static void Load(const Json::Value& val, Sprite* spr);
	static void Store(Json::Value& val, const Sprite* spr);

private:
	static void LoadGeometry(const Json::Value& val, Sprite* spr);
	static void StoreGeometry(Json::Value& val, const Sprite* spr);

	static void LoadShader(const Json::Value& val, Sprite* spr);
	static void StoreShader(Json::Value& val, const Sprite* spr);

	static void LoadInfo(const Json::Value& val, Sprite* spr);
	static void StoreInfo(Json::Value& val, const Sprite* spr);

	static void LoadEdit(const Json::Value& val, Sprite* spr);
	static void StoreEdit(Json::Value& val, const Sprite* spr);

	static void LoadColor(const Json::Value& val, s2::RenderColor& color);
	static void StoreColor(Json::Value& val, const s2::RenderColor& color);

	static void LoadShader(const Json::Value& val, s2::RenderShader& shader);
	static void StoreShader(Json::Value& val, const s2::RenderShader& shader);	

	static void LoadCamera(const Json::Value& val, s2::RenderCamera& camera);
	static void StoreCamera(Json::Value& val, const s2::RenderCamera& camera);	

}; // SpriteIO

}

#endif // _EASYEDITOR_SPRITE_IO_H_