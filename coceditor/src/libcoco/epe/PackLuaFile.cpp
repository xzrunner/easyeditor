#include "PackLuaFile.h"
#include "TexturePacker.h"
#include "CocoPacker.h"

#include "../SymbolDependanceSorter.h"

#include <ee/ImageSymbol.h>

#include <easycomplex.h>

namespace ecoco
{
namespace epe
{

void PackLuaFile::pack(const std::vector<const ee::Sprite*>& sprites, 
	const std::string& outfloder)
{
	// root symbol
	ecomplex::Symbol root;
	root.name = "root";
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		ee::Sprite* sprite = const_cast<ee::Sprite*>(sprites[i]);
		sprite->Retain();
		root.m_sprites.push_back(sprite);
	}

	// get all symbols
	SymbolDependanceSorter preprocess;
	preprocess.prepare(sprites);
	std::vector<const ee::Symbol*> symbols = preprocess.GetSymbolSet().GetOrdered();
	symbols.push_back(&root);

	// pack images
	std::set<ee::Image*> setImages;
	for (int i = 0, n = symbols.size(); i < n; ++i)
	{
		if (const ee::ImageSymbol* s = dynamic_cast<const ee::ImageSymbol*>(symbols[i]))
			setImages.insert(s->GetImage());
	}

	TexturePacker textures;
	textures.Pack(setImages);
	textures.StoreToMemory();
	textures.StoreToFile(outfloder, "tmp.1", ee::ImageSaver::e_ppm);

	// pack lua file
	CocoPacker code(textures);
	code.pack(symbols);
	code.storeToFile(outfloder + "\\tmp.lua");
}

}
}