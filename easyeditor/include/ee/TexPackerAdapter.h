#ifndef _EASYEDITOR_TEXPACKER_ADAPTER_H_
#define _EASYEDITOR_TEXPACKER_ADAPTER_H_

#include <json/json.h>

namespace ee
{

class TexPackerAdapter
{
public:
	void Load(const char* filename);

public:
	struct Region
	{
		float left, low;
		float width, height;
	};

	struct Texture
	{
		std::string filepath;

		Region region;

		bool bRotate;
	};

private:
	void load(const Json::Value& value, const std::string& dir);

public:
	int width, height;

	std::vector<Texture> textures;

}; // TexPackerAdapter

}

#endif // _EASYEDITOR_TEXPACKER_ADAPTER_H_