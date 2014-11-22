#include "PackLuaFile.h"
#include "TexturePacker.h"
#include "CocoPacker.h"
#include "SymbolsPrepare.h"

#include <easycomplex.h>

namespace libcoco
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
	SymbolsPrepare preprocess;
	preprocess.prepare(sprites);
	std::vector<const d2d::ISymbol*> symbols = preprocess.GetAllSymbols();
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