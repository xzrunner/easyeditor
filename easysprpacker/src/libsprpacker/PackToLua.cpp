#include "PackToLua.h"
#include "PackNode.h"
#include "PackNodeFactory.h"
#include "PackIDMgr.h"

#include <ee/ImageData.h>

#include <easybuilder.h>

#include <simp/NodeID.h>

namespace esprpacker
{
	
class PackNodeCmp
{
public:
	bool operator () (const PackNode* lhs, const PackNode* rhs) const {
		return lhs->GetID() < rhs->GetID();
	}
}; // PackNodeCmp

void PackToLua::Pack(const std::string& filepath, const ee::TexturePacker& tp)
{
	std::vector<PackNode*> nodes;
	PackNodeFactory::Instance()->FetchAll(nodes);
	std::sort(nodes.begin(), nodes.end(), PackNodeCmp());
	
	uint32_t default_sym_id = 0xffffffff;
	const std::string default_sym_path = ee::ImageDataMgr::Instance()->GetDefaultSym();
	if (!default_sym_path.empty()) {
		int pkg_id, node_id;
		PackIDMgr::Instance()->QueryID(default_sym_path, pkg_id, node_id, true);
		default_sym_id = simp::NodeID::ComposeID(pkg_id, node_id);
	}

	ebuilder::CodeGenerator gen;
	gen.line("return {");
	for (auto& node : nodes) {
		if (!PackIDMgr::Instance()->IsCurrPkg(node)) {
			continue;
		}
		if (default_sym_id != 0xffffffff && default_sym_id == node->GetID() &&
			default_sym_path != node->GetFilepath()) {
			continue;
		}
		node->PackToLuaString(gen, tp);
	}
	gen.line("}");

	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));
	fout << gen.toText() << std::endl;
	fout.close();
}

}