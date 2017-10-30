#include "DBHelper.h"
#include "Database.h"
#include "LeafNode.h"
#include "JsonResDelOP.h"
#include "JsonResFixOP.h"
#include "JsonResRenameOP.h"
#include "JsonResChangePathOP.h"
#include "JsonResChangeDirOP.h"

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
		dst = gum::FilepathHelper::Format(dst);
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

void DBHelper::FixNodeRef(const Database& db, const LeafNode* removed)
{
	const std::set<int>& list = removed->GetNodes(true);
	if (list.empty()) {
		return;
	}

	std::string rm_path = db.GetDirPath() + "\\" + removed->GetPath();
 	std::set<int>::const_iterator itr = list.begin();
 	for ( ; itr != list.end(); ++itr) 
	{
		const Node* node = db.Fetch(*itr);
		if (node->Type() != NODE_LEAF) {
			continue;
		}

		std::string path = db.GetDirPath() + "\\" + node->GetPath();

		const LeafNode* leaf = static_cast<const LeafNode*>(node);
		const_cast<std::set<int>>(leaf->GetNodes(false)).erase(removed->GetID());

		JsonResDelOP del_op(path, rm_path);
		del_op.Do();
		JsonResFixOP fix_op(path);
		fix_op.Do();
 	}
}

void DBHelper::RenameNode(const Database& db, const LeafNode* node, 
						  const std::string& new_name)
{
	std::string old_path = db.GetDirPath() + "\\" + node->GetPath();
	std::string dir = gum::FilepathHelper::Dir(old_path);
	std::string old_name = gum::FilepathHelper::Filename(old_path);
	std::string new_path = dir + "\\" + new_name;
	wxRenameFile(old_path, new_path);

	const_cast<Database&>(db).RenamePath(old_path, new_path);

	std::string new_node_path = gum::FilepathHelper::Relative(db.GetDirPath(), new_path);
	const_cast<LeafNode*>(node)->SetPath(new_node_path);

	const std::set<int>& list = node->GetNodes(true);
	if (list.empty()) {
		return;
	}

	std::set<int>::const_iterator itr = list.begin();
	for ( ; itr != list.end(); ++itr) 
	{
		const Node* node = db.Fetch(*itr);
		if (node->Type() != NODE_LEAF) {
			continue;
		}
	
		std::string path = db.GetDirPath() + "\\" + node->GetPath();
		JsonResRenameOP op(path, old_name, new_name);
		op.Do();
	}
}

void DBHelper::MoveNode(const Database& db, const LeafNode* node, 
						const std::string& dst_dir)
{
	std::string old_path = db.GetDirPath() + "\\" + node->GetPath();
	std::string dir = gum::FilepathHelper::Dir(old_path);
	std::string old_name = gum::FilepathHelper::Filename(old_path);
	std::string new_path = dst_dir + "\\" + old_name;

	const_cast<Database&>(db).RenamePath(old_path, new_path);

	std::string new_node_path = gum::FilepathHelper::Relative(db.GetDirPath(), new_path);
	const_cast<LeafNode*>(node)->SetPath(new_node_path);

	const std::set<int>& list = node->GetNodes(true);
	std::set<int>::const_iterator itr = list.begin();
	for ( ; itr != list.end(); ++itr) 
	{
		const Node* node = db.Fetch(*itr);
		if (node->Type() != NODE_LEAF) {
			continue;
		}

		std::string path = db.GetDirPath() + "\\" + node->GetPath();
		if (path != new_path) {
			JsonResChangePathOP op(path, old_path, new_path);
			op.Do();
		}
	}

	wxCopyFile(old_path, new_path);
	wxRemoveFile(old_path);
}

void DBHelper::CopyNode(const Database& db, const LeafNode* node, 
						const std::string& dst_dir)
{
	std::string old_path = db.GetDirPath() + "\\" + node->GetPath();
	std::string dir = gum::FilepathHelper::Dir(old_path);
	std::string old_name = gum::FilepathHelper::Filename(old_path);
	std::string old_dir = gum::FilepathHelper::Dir(old_path);
	std::string new_path = dst_dir + "\\" + old_name;

	wxCopyFile(old_path, new_path);

	JsonResChangeDirOP op(new_path, old_dir, dst_dir);
	op.Do();
}

void DBHelper::ChangeNode(const Database& db, const LeafNode* node, 
						  const std::string& new_node_path)
{
	std::string old_path = db.GetDirPath() + "\\" + node->GetPath();
	std::string dir = gum::FilepathHelper::Dir(old_path);

	int new_node_id = db.QueryByPath(new_node_path);
	const Node* new_node = db.Fetch(new_node_id);
	assert(new_node);
	LeafNode* new_leaf = const_cast<LeafNode*>(static_cast<const LeafNode*>(new_node));

	const std::set<int>& list = node->GetNodes(true);
	std::set<int>::const_iterator itr = list.begin();
	for ( ; itr != list.end(); ++itr) 
	{
		const Node* node = db.Fetch(*itr);
		if (node->Type() != NODE_LEAF) {
			continue;
		}

		std::string path = db.GetDirPath() + "\\" + node->GetPath();
		if (path != new_node_path) 
		{
			new_leaf->AddInput(*itr);
			JsonResChangePathOP op(path, old_path, new_node_path);
			op.Do();
		}
	}
	const_cast<LeafNode*>(node)->ClearInput();
}

}