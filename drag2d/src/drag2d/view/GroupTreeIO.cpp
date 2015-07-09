#include "GroupTreeIO.h"
#include "GroupTreeCtrl.h"

#include "dataset/ISprite.h"
#include "view/MultiSpritesImpl.h"

#include <queue>

namespace d2d
{

GroupTreeIO::GroupTreeIO(GroupTreeCtrl* tree, MultiSpritesImpl* sprites_impl)
	: m_tree(tree)
	, m_sprites_impl(sprites_impl)
{
}

void GroupTreeIO::Load(const Json::Value& value)
{
	std::vector<ISprite*> sprites;
	m_sprites_impl->TraverseSprites(FetchAllVisitor<ISprite>(sprites));

	std::vector<Node> nodes;
	LoadToNodes(value, nodes);

	wxTreeItemId root_id = m_tree->GetRootID();

	std::string parent_name = "root";
	wxTreeItemId parent_id = root_id;

	std::queue<std::pair<std::string, wxTreeItemId> > candidates;
	candidates.push(std::make_pair(parent_name, parent_id));
	
	while (!candidates.empty())
	{
		std::pair<std::string, wxTreeItemId> candidate = candidates.front(); candidates.pop();

 		std::vector<Node>::iterator itr = nodes.begin();
 		for ( ; itr != nodes.end(); ) {
 			const Node& node = *itr;
 			if (node.parent == candidate.first) {
				if (!node.sprite.empty()) {
					ISprite* spr = NULL;

					for (int i = 0, n = sprites.size(); i < n; ++i) {
						if (node.name == sprites[i]->name) {
							spr = sprites[i];
							break;
						}
					}

					if (spr) {
						wxTreeItemId id = m_tree->AddSprite(candidate.second, spr);
						if (id != root_id) {
							candidates.push(std::make_pair(node.name, id));
						}
					}
				} else {
					wxTreeItemId id = m_tree->AddNode(node.name, candidate.second, 
						node.visible, node.editable);
					candidates.push(std::make_pair(node.name, id));
				}
				itr = nodes.erase(itr);
			} else {
				++itr;
			}
 		}

		if (nodes.empty()) {
			break;
		}
	}
}

void GroupTreeIO::LoadToNodes(const Json::Value& value, std::vector<Node>& nodes)
{
	int idx = 0;
	Json::Value node_val = value["node"][idx++];
	while (!node_val.isNull()) {
		Node node;
		node.is_leaf = node_val["leaf"].asBool();
		node.name = node_val["name"].asString();
		node.parent = node_val["parent"].asString();
		node.sprite = node_val["sprite"].asString();
		if (!node_val["visible"].isNull()) {
			node.visible = node_val["visible"].asBool();
		} else {
			node.visible = true;
		}
		if (!node_val["editable"].isNull()) {
			node.editable = node_val["editable"].asBool();
		} else {
			node.editable = true;
		}

		if (node.name != "root") {
			nodes.push_back(node);
		}
		node_val = value["node"][idx++];
	}
}

}