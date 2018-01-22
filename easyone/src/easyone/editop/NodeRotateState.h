#pragma once

#include "editop/EditOpState.h"
#include "data/SceneNode.h"

#include <ee/Visitor.h>
#include <ee/SelectionSet.h>

namespace eone
{

class NodeRotateState : public EditOpState
{
public:
	NodeRotateState(const ee::SelectionSet<SceneNode>& selection);

	virtual void OnMousePress(const sm::vec2& pos) override;
	virtual void OnMouseRelease(const sm::vec2& pos) override;
	virtual void OnMouseDrag(const sm::vec2& pos) override;

private:
	void Rotate(const sm::vec2& start, const sm::vec2& end);

private:
	class Visitor : public ee::RefVisitor<SceneNode>
	{
	public:
		Visitor(const sm::vec2& start, const sm::vec2& end)
			: m_start(start), m_end(end) {}
		virtual void Visit(const SceneNodePtr& node, bool& next) override;

	private:
		sm::vec2 m_start, m_end;

	}; // Visitor

private:
	const ee::SelectionSet<SceneNode>& m_selection;

	sm::vec2 m_last_pos;

}; // NodeRotateState

}