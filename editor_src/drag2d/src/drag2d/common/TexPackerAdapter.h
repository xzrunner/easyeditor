#pragma once

#include <json/json.h>

#include "IFileAdapter.h"

namespace d2d
{
	class TexPackerAdapter : public IFileAdapter
	{
	public:
		virtual void load(const char* filename);

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
		void load(const Json::Value& value, const std::string& dlg);

	public:
		int width, height;

		std::vector<Texture> textures;

	}; // TexPackerAdapter
}

