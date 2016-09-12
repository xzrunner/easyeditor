#include "PackToLua.h"
#include "PackNode.h"
#include "PackNodeFactory.h"

#include <easybuilder.h>

namespace esprpacker
{
	
class PackNodeCmp
{
public:
	bool operator () (const PackNode* lhs, const PackNode* rhs) const {
		return lhs->GetID() < rhs->GetID();
	}
}; // PackNodeCmp

void PackToLua::Pack(const std::string& filepath, const ee::TexturePacker& tp, float scale)
{
	std::vector<PackNode*> nodes;
	PackNodeFactory::Instance()->FetchAll(nodes);
	std::sort(nodes.begin(), nodes.end(), PackNodeCmp());

	ebuilder::CodeGenerator gen;
	gen.line("return {");
	for (int i = 0, n = nodes.size(); i < n; ++i) {
		nodes[i]->PackToLuaString(gen, tp, scale);
	}
	gen.line("}");

	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));
	fout << gen.toText() << std::endl;
	fout.close();
}

}