#include "ParserLuaData.h"

#include <cu/cu_stl.h>
#include <simp/Package.h>
#include <simp/PkgIdxLoader.h>
#include <simp/Page.h>
#include <simp/PageAlloc.h>
#include <simp/simp_define.h>
#include <simp/NodeVisitor.h>

namespace esprpacker
{

class NodeVisitor : public simp::NodeVisitor
{
public:
	virtual void Visit(int id, int type, const void* node) override {
		printf("id %d, type %d, node %p\n", id, type, node);
	}

}; // NodeVisitor

void ParserLuaData::LoadFromFile(const std::string& filepath)
{
	bimp::Allocator* alloc = simp::PageAlloc::Instance()->Create(simp::SIMP_PAGE_SIZE_MAX);
	if (!alloc) {
		return;
	}

	simp::Package* pkg = new simp::Package(filepath.c_str(), -1);
	int version = pkg->GetVersion();
	auto& pages = pkg->GetPages();
	int idx = 1;
	for (auto& page : pages)
	{
		auto page_data = std::make_unique<simp::Page>(-1, version, alloc, page.MinID(), page.MaxID());
		std::string page_path = filepath;
		page_path.insert(page_path.rfind(".epe"), ".1");
		page_data->Load(bimp::FilePath(page_path.c_str()));
		NodeVisitor visitor;
		page_data->Traverse(visitor);

		++idx;
	}
}

}