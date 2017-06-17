#include "DBHelper.h"
#include "Database.h"
#include "LeafNode.h"

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

}