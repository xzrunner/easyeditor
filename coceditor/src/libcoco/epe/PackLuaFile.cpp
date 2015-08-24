#include "PackLuaFile.h"
#include "TexturePacker.h"
#include "CocoPacker.h"
#include "SymbolDependanceSorter.h"

#include <easycomplex.h>

namespace libcoco
{
namespace epe
{

void PackLuaFile::pack(const std::vector<const d2d::ISprite*>& sprites, 
	const std::string& outfloder)
{
	// root symbol
	ecomplex::Symbol root;
	root.name = "root";
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = const_cast<d2d::ISprite*>(sprites[i]);
		sprite->Retain();
		root.m_sprites.push_back(sprite);
	}

	// get all symbols
	SymbolDependanceSorter preprocess;
	preprocess.prepare(sprites);
	std::vector<const d2d::ISymbol*> symbols = preprocess.GetSymbolSet().GetOrdered();
	symbols.push_back(&root);

	// pack images
	std::set<d2d::Image*> setImages;
	for (int i = 0, n = symbols.size(); i < n; ++i)
	{
		if (const d2d::ImageSymbol* s = dynamic_cast<const d2d::ImageSymbol*>(symbols[i]))
			setImages.insert(s->getImage());
	}

	TexturePacker textures;
	textures.pack(setImages);
	textures.storeToMemory();
	textures.storeToFile(outfloder, "tmp.1", d2d::ImageSaver::e_ppm);

	// pack lua file
	CocoPacker code(textures);
	code.pack(symbols);
	code.storeToFile(outfloder + "\\tmp.lua");
}

}
}