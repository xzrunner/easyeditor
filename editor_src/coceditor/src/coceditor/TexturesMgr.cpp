#include "TexturesMgr.h"

namespace coceditor
{

TexturesMgr::TexturesMgr()
{
	memset(textures, 0, sizeof(int)*NUM);
}

TexturesMgr::~TexturesMgr()
{
	for (size_t i = 0; i < NUM; ++i)
		delete textures[i];
}

bool TexturesMgr::initTexture(const std::string& filepath, int index)
{
	if (index >= NUM) return false;

	for (size_t i = 0; i < NUM; ++i)
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