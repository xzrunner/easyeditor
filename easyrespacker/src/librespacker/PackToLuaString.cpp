#include "PackToLuaString.h"
#include "PackNodeFactory.h"
#include "IPackNode.h"

#include <easybuilder.h>

namespace librespacker
{

void PackToLuaString::Pack(const std::string& filepath,
						   const d2d::TexturePacker& tp,
						   float scale)
{
	std::vector<IPackNode*> nodes;
	PackNodeFactory::Instance()->GetAllNodes(nodes);
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