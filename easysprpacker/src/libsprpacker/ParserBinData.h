#pragma once

#include <simp/NodeVisitor.h>

#include <map>
#include <string>
#include <vector>
#include <set>

namespace esprpacker
{

class ParserBinData
{
public:
	void LoadFromFile(const std::string& filepath);

	void InitSprIDSet(int pkg_id);

	bool IsIdUsed(uint32_t id) const;

//	bool QuerySprID();

private:
	struct NodeSpr
	{
		NodeSpr(int id, const void* node)
			: id(id), node(node) {}
		int id;
		const void* node;
	};

	class NodeVisitor : public simp::NodeVisitor
	{
	public:
		NodeVisitor(std::map<uint32_t, std::vector<ParserBinData::NodeSpr>>& spr_nodes)
			: m_spr_nodes(spr_nodes) {}

		virtual void Visit(int id, int type, const void* node) override;

	private:
		void InsertSpr(uint32_t sym_id, int spr_id, const void* spr_node);

	private:
		std::map<uint32_t, std::vector<ParserBinData::NodeSpr>>& m_spr_nodes;

	}; // NodeVisitor


private:
	std::map<uint32_t, std::vector<NodeSpr>> m_spr_nodes;

	std::set<uint32_t> m_spr_id_set;

}; // ParserBinData

}