#pragma once

#include <json/json.h>

#include "IFileAdapter.h"

namespace d2d
{
	class TexturePackerFileAdapter : public IFileAdapter
	{
	public:
		virtual void load(const char* filename);

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

		std::vector<Entry> frames;

	}; // TexturePackerFileAdapter
}

