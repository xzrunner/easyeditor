#pragma once

#include <drag2d.h>

namespace coceditor
{
	class TexturesMgr
	{
	public:
		struct Entry
		{
			std::string filepath;
			d2d::TexturePackerFileAdapter adapter;
		};

	public:
		TexturesMgr();
		~TexturesMgr();

		bool initTexture(const std::string& filepath, int index);

	public:
		static const int NUM = 10;

	public:
		Entry* textures[NUM];

	}; // TexturesMgr
}

