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

void PackLuaFile::pack(const std::vector<ee::SprPtr>& sprs, const std::string& outfloder)
{
	// root symbol
	auto root = std::make_shared<ecomplex::Symbol>();
	root->name = "root";
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		root->Add(sprs[i]);
	}

	// get all syms
	SymbolDependanceSorter preprocess;
	preprocess.prepare(sprs);
	auto syms = preprocess.GetSymbolSet().GetOrdered();
	syms.push_back(root);

	// pack images
	std::set<std::shared_ptr<ee::Image>> setImages;
	for (int i = 0, n = syms.size(); i < n; ++i)
	{
		if (auto s = std::dynamic_pointer_cast<const ee::ImageSymbol>(syms[i]))
			setImages.insert(s->GetImage());
	}

	TexturePacker textures;
	textures.Pack(setImages);
	textures.StoreToMemory();
	textures.StoreToFile(outfloder, "tmp.1");

	// pack lua file
	CocoPacker code(textures);
	code.pack(syms);
	code.storeToFile(outfloder + "\\tmp.lua");
}

}
}