#ifndef _EASYEDITOR_SPRITE_IO_H_
#define _EASYEDITOR_SPRITE_IO_H_

#include <json/json.h>

namespace s2 { class RenderColor; class RenderShader; }

namespace ee
{

class SpriteIO
{
public:
	static void LoadColor(const Json::Value& val, s2::RenderColor& color);
	static void StoreColor(Json::Value& val, const s2::RenderColor& color);

	static void LoadShader(const Json::Value& val, s2::RenderShader& shader);
	static void StoreShader(Json::Value& val, const s2::RenderShader& shader);	
	
}; // SpriteIO

}

#endif // _EASYEDITOR_SPRITE_IO_H_