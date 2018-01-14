#include "NodeSelection.h"

#include <ee/panel_msg.h>

namespace enode3d
{

void NodeSelection::Clear()
{
	ee::SelectionSet<n3::INode>::Clear();
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void NodeSelection::Add(const n3::NodePtr& node)
{
	ee::SelectionSet<n3::INode>::Add(node);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void NodeSelection::Remove(const n3::NodePtr& node)
{
	ee::SelectionSet<n3::INode>::Remove(node);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}