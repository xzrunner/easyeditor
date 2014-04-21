#pragma once

#include <drag2d.h>

#include "TexPackerAdapter.h"

namespace coceditor
{
	class TexturesMgr
	{
	public:
		struct Entry
		{
			std::string filepath;
			TexPackerAdapter adapter;
		};

	public:
		TexturesMgr();
		~TexturesMgr();

		bool initTexture(const std::string& filepath, int index);

	public:
		static const int MAX_NUM = 10;

	public:
		Entry* textures[MAX_NUM];

	}; // TexturesMgr
}

