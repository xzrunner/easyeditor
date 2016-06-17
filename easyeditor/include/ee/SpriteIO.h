#ifndef _EASYEDITOR_SPRITE_IO_H_
#define _EASYEDITOR_SPRITE_IO_H_

#include <SM_Vector.h>
#include <sprite2/BlendMode.h>
#include <sprite2/FilterMode.h>
#include <sprite2/CameraMode.h>
#include <sprite2/RenderColor.h>

#include <json/json.h>

namespace s2 { class RenderShader; class RenderCamera; class RenderFilter; }

namespace ee
{

class Sprite;

class SpriteIO
{
public:
	static void Load(const Json::Value& val, Sprite* spr);
	static void Store(Json::Value& val, const Sprite* spr);

public:
	struct Data
	{
		Data();
		// pass filter to spr
//		~Data() { delete filter; }

		// geometry
		sm::vec2		position;
		float			angle;
		sm::vec2		scale;
		sm::vec2		shear;
		sm::vec2		offset;
		sm::bvec2		mirror;
		sm::vec2		perspective;

		// shader
		s2::RenderColor	col;
 		s2::BlendMode	blend;
		s2::RenderFilter* filter;
		s2::CameraMode	camera;

		// info
		std::string		name;
		std::string		tag;
		bool			clip;
		bool			anchor;

		// edit
		bool			visible;
		bool			editable;

	}; // Data

	static void Load(const Json::Value& val, Data& data);
	static void Store(Json::Value& val, const Data& data);	

private:
	static void LoadGeometry(const Data& data, Sprite* spr);
	static void StoreGeometry(Data& data, const Sprite* spr);
	static void LoadGeometry(const Json::Value& val, Data& data);
	static void StoreGeometry(Json::Value& val, const Data& data);

	static void LoadRender(const Data& data, Sprite* spr);
	static void StoreRender(Data& data, const Sprite* spr);
	static void LoadRender(const Json::Value& val, Data& data);
	static void StoreRender(Json::Value& val, const Data& data);

	static void LoadInfo(const Data& data, Sprite* spr);
	static void StoreInfo(Data& data, const Sprite* spr);
	static void LoadInfo(const Json::Value& val, Data& data);
	static void StoreInfo(Json::Value& val, const Data& data);

	static void LoadEdit(const Data& data, Sprite* spr);
	static void StoreEdit(Data& data, const Sprite* spr);
	static void LoadEdit(const Json::Value& val, Data& data);
	static void StoreEdit(Json::Value& val, const Data& data);

	static void LoadColor(const Data& data, s2::RenderColor& color);
	static void StoreColor(Data& data, const s2::RenderColor& color);
	static void LoadColor(const Json::Value& val, Data& data);
	static void StoreColor(Json::Value& val, const Data& data);

	static void LoadShader(const Data& data, s2::RenderShader& shader);
	static void StoreShader(Data& data, const s2::RenderShader& shader);	
	static void LoadShader(const Json::Value& val, Data& data);
	static void StoreShader(Json::Value& val, const Data& data);	

	static void LoadCamera(const Data& data, s2::RenderCamera& camera);
	static void StoreCamera(Data& data, const s2::RenderCamera& camera);	
	static void LoadCamera(const Json::Value& val, Data& data);
	static void StoreCamera(Json::Value& val, const Data& data);

}; // SpriteIO

}

#endif // _EASYEDITOR_SPRITE_IO_H_