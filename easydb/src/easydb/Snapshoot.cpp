#include "Snapshoot.h"
#include "Node.h"
#include "Database.h"
#include "LeafNode.h"

#include <ee/SymbolMgr.h>
#include <ee/Symbol.h>

#include <sprite2/DrawRT.h>
#include <gum/FilepathHelper.h>

#include <wx/dir.h>
#include <wx/filename.h>

#include <vector>

namespace edb
{

void Snapshoot::Build(const Database& db, const std::string& dir)
{
	const std::vector<Node*>& nodes = db.GetNodes();
	for (int i = 0, n = nodes.size();  i < n; ++i)
	{
		Node* node = nodes[i];
		if (node->Type() != NODE_LEAF) {
			continue;
		}

		LeafNode* leaf = static_cast<LeafNode*>(node);
		const std::string& path = leaf->GetPath();

		std::string out_path = dir + "\\" + path;
		out_path = out_path.substr(0, out_path.find_last_of("."));
		out_path += "_ss.png";

		std::string dir = gum::FilepathHelper::Dir(out_path);
		if (!wxDir::Exists(dir)) {
			wxFileName::Mkdir(dir, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
		}

		s2::DrawRT rt;
		std::string ori_path = db.GetDirPath() + "\\" + path;
		auto sym = ee::SymbolMgr::Instance()->FetchSymbol(ori_path);
		rt.Draw(*sym);

		sm::vec2 sz = sym->GetBounding().Size();
		rt.StoreToFile(out_path, sz.x, sz.y);
	}
}

}