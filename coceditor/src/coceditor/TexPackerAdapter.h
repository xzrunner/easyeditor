#pragma once

#include <json/json.h>

namespace coceditor
{

class TexPackerAdapter
{
public:
	void load(const char* filename);

public:
	struct Region
	{
		int x, y;
		int w, h;
	};

	struct Entry
	{
		std::string filename;

		Region frame;

		bool rotated;

		bool trimmed;
		Region spriteSourceSize;

		int srcWidth, srcHeight;
	};

private:
	void load(const Json::Value& value, Entry& entry);
	void load(const Json::Value& value, Region& region);

public:
	int width, height;

	float invscale;

	std::vector<Entry> frames;

}; // TexturePackerFileAdapter

}

