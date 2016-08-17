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

void PackLuaFile::pack(const std::vector<const ee::Sprite*>& sprs, 
	const std::string& outfloder)
{
	// root symbol
	ecomplex::Symbol root;
	root.name = "root";
	for (int i = 0, n = sprs.size(); i < n; ++i)
	{
		ee::Sprite* spr = const_cast<ee::Sprite*>(sprs[i]);
		spr->AddReference();
		root.Add(spr);
	}

	// get all syms
	SymbolDependanceSorter preprocess;
	preprocess.prepare(sprs);
	std::vector<const ee::Symbol*> syms = preprocess.GetSymbolSet().GetOrdered();
	syms.push_back(&root);

	// pack images
	std::set<ee::Image*> setImages;
	for (int i = 0, n = syms.size(); i < n; ++i)
	{
		if (const ee::ImageSymbol* s = dynamic_cast<const ee::ImageSymbol*>(syms[i]))
			setImages.insert(s->GetImage());
	}

	TexturePacker textures;
	textures.Pack(setImages);
	textures.StoreToMemory();
	textures.StoreToFile(outfloder, "tmp.1", ee::ImageSaver::e_ppm);

	// pack lua file
	CocoPacker code(textures);
	code.pack(syms);
	code.storeToFile(outfloder + "\\tmp.lua");
}

}
}