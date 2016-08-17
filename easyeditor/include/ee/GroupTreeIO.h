#ifndef _EASYEDITOR_GROUP_TREE_IO_H_
#define _EASYEDITOR_GROUP_TREE_IO_H_

#include <json/json.h>

namespace ee
{

class GroupTreeCtrl;
class MultiSpritesImpl;

class GroupTreeIO
{
public:
	GroupTreeIO(GroupTreeCtrl* tree, MultiSpritesImpl* sprites_impl);

	void Load(const Json::Value& value);

private:
	struct Node
	{
		bool is_leaf;
		std::string name;
		std::string parent;
		std::string spr;
		bool visible, editable;
	}; 

private:
	void LoadToNodes(const Json::Value& value, std::vector<Node>& nodes);

private:
	GroupTreeCtrl* m_tree;

	MultiSpritesImpl* m_sprites_impl;

}; // GroupTreeIO

}

#endif // _EASYEDITOR_GROUP_TREE_IO_H_