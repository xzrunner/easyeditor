#include "DBHelper.h"
#include "Database.h"
#include "LeafNode.h"

#include <gum/FilepathHelper.h>

#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/filename.h>

#include <queue>

namespace edb
{

bool DBHelper::IsTreeClosure(const Database& db, const LeafNode* root)
{
	const std::set<int>& out = root->GetNodes(false);
	if (out.empty()) {
		return true;
	}

	std::set<int> closure;
	closure.insert(root->GetID());

	std::queue<int> buffer;
	for (std::set<int>::const_iterator itr = out.begin(); itr != out.end(); ++itr) {
		buffer.push(*itr);
	}
	while (!buffer.empty())
	{
		int id = buffer.front(); buffer.pop();
		const Node* child = db.Fetch(id);
		if (!child || child->Type() == NODE_INDEX) {
			break;
		}

		const LeafNode* child_leaf = static_cast<const LeafNode*>(child);	

		const std::set<int>& child_in = child_leaf->GetNodes(true);
		for (std::set<int>::const_iterator itr = child_in.begin(); itr != child_in.end(); ++itr) 
		{
			if (closure.find(*itr) == closure.end()) {
				return false;
			}
		}

		const std::set<int>& child_out = child_leaf->GetNodes(false);
		for (std::set<int>::const_iterator itr = child_out.begin(); itr != child_out.end(); ++itr) {
			buffer.push(*itr);
		}

		closure.insert(id);
	}

	return true;
}

void DBHelper::CopyTree(const Database& db, const LeafNode* root, const std::string& dst_dir)
{
	std::string base_dir = gum::FilepathHelper::Dir(db.GetDirPath() + "\\" + root->GetPath());

	std::queue<int> buffer;
	buffer.push(root->GetID());
	while (!buffer.empty())
	{
		int id = buffer.front(); buffer.pop();
		const Node* node = db.Fetch(id);
		if (!node || node->Type() == NODE_INDEX) {
			continue;
		}

		const LeafNode* leaf = static_cast<const LeafNode*>(node);	
		std::string src = db.GetDirPath() + "\\" + leaf->GetPath();
		std::string relative = gum::FilepathHelper::Relative(base_dir, src);
		std::string dst = dst_dir + "\\" + relative;
		std::string _dst_dir = gum::FilepathHelper::Dir(dst);
		if (!wxDir::Exists(_dst_dir)) {
			wxFileName::Mkdir(_dst_dir, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
		}
		wxCopyFile(src, dst);
		
		const std::set<int>& child_out = leaf->GetNodes(false);
		for (std::set<int>::const_iterator itr = child_out.begin(); itr != child_out.end(); ++itr) {
			buffer.push(*itr);
		}
	}
}

void DBHelper::DeleteTree(const Database& db, int root)
{
	std::queue<int> buffer;
	buffer.push(root);
	while (!buffer.empty())
	{
		int id = buffer.front(); buffer.pop();
		const Node* node = db.Fetch(id);
		if (!node || node->Type() == NODE_INDEX) {
			continue;
		}

		const LeafNode* leaf = static_cast<const LeafNode*>(node);
		wxRemoveFile(db.GetDirPath() + "\\" + leaf->GetPath());
		
		const std::set<int>& child_out = leaf->GetNodes(false);
		for (std::set<int>::const_iterator itr = child_out.begin(); itr != child_out.end(); ++itr) {
			buffer.push(*itr);
		}
	}
}

}