#include "LoadFromBin.h"
#include "Symbol.h"

#include <ee/SymbolMgr.h>
#include <ee/SpriteFactory.h>
#include <ee/FileHelper.h>

#include <bsn/NodeFactory.h>
#include <bsn/NodeSym.h>
#include <bsn/NodeSpr.h>
#include <bsn/ComplexSym.h>
#include <memmgr/LinearAllocator.h>
#include <bs/ImportStream.h>

#include <memory>
#include <fstream>

namespace ecomplex
{

void LoadFromBin::Load(const std::string& filepath, Symbol& complex)
{
	mm::LinearAllocator alloc;

	std::ifstream fin(filepath, std::ios::binary);
	fin.seekg(0, std::ios::end);
	size_t len = static_cast<size_t>(fin.tellg());
	fin.seekg(0, std::ios::beg);
	char* data = new char[len];
	fin.read(data, len);
	fin.close();

	auto sym = bsn::NodeFactory::CreateNodeSym(alloc, bs::ImportStream(data, len));
	auto sym_src = dynamic_cast<bsn::ComplexSym*>(sym);

	sm::rect scissor;
	int16_t xmin, ymin, xmax, ymax;
	sym_src->GetScissor(xmin, ymin, xmax, ymax);
	complex.SetScissor(scissor);

	auto dir = ee::FileHelper::GetFileDir(filepath);
	for (int i = 0, n = sym_src->GetChildrenNum(); i < n; ++i) 
	{
		auto child = sym_src->GetChildByIndex(i);
		auto child_path = ee::FileHelper::GetAbsolutePath(
			dir, child->GetBaseInfo().GetFilepath());
		auto sym = ee::SymbolMgr::Instance()->FetchSymbol(child_path);
		auto spr = ee::SpriteFactory::Instance()->Create(sym);
		spr->Load(child);
		spr->SetVisible(true);
		complex.Add(spr);
	}
}

}