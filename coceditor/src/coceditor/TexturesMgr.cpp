#include "TexturesMgr.h"

namespace coceditor
{

TexturesMgr::TexturesMgr()
{
	memset(textures, 0, sizeof(int)*MAX_NUM);
}

TexturesMgr::~TexturesMgr()
{
	for (size_t i = 0; i < MAX_NUM; ++i)
		delete textures[i];
}

bool TexturesMgr::initTexture(const std::string& filepath, int index)
{
	if (index >= MAX_NUM) return false;

	for (size_t i = 0; i < MAX_NUM; ++i)
		if (textures[i] && textures[i]->filepath == filepath)
			return false;

	Entry* entry = new Entry;
	entry->filepath = filepath;
	entry->adapter.load(filepath.c_str());
	delete textures[index];
	textures[index] = entry;
	return true;
}
} // coceditor