#pragma once

#include <ee/SelectionSet.h>

#include <node3/INode.h>

namespace enode3d
{

class NodeSelection : public ee::SelectionSet<n3::INode>
{
public:
	virtual void Clear() override;

	virtual void Add(const n3::NodePtr& node) override;
	virtual void Remove(const n3::NodePtr& node) override;

}; // NodeSelection

}