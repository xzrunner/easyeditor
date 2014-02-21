#include "PackLuaFile.h"
#include "TexturePacker.h"
#include "CocoPacker.h"

namespace libcoco
{

void PackLuaFile::pack(const std::vector<const d2d::ISprite*>& sprites, 
	const std::string& outfloder)
{
	// pack images
	std::set<d2d::Image*> setImages;
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		if (const d2d::ImageSprite* s = dynamic_cast<const d2d::ImageSprite*>(sprites[i]))
			setImages.insert(s->getSymbol().getImage());
	}
	TexturePacker textures;
	textures.pack(setImages);
	textures.storeToMemory();
	textures.storeToFile(outfloder, "tmp.1", d2d::ImageSaver::e_ppm);

	// pack lua file
	CocoPacker code(textures);
	code.pack(sprites);
	code.storeToFile(outfloder + "\\tmp.lua");
}

}