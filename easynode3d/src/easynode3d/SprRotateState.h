#pragma once

#include "EditOpState.h"

#include <ee/Visitor.h>

#include <node3/INode.h>

namespace enode3d
{

class StageCanvas;
class Sprite;
class NodeSelection;

class SprRotateState : public EditOpState
{
public:
	SprRotateState(StageCanvas& canvas, const NodeSelection& selection);

	virtual void OnMousePress(const sm::vec2& pos) override;
	virtual void OnMouseRelease(const sm::vec2& pos) override;
	virtual void OnMouseDrag(const sm::vec2& pos) override;

private:
	void Rotate(const sm::vec2& start, const sm::vec2& end);

private:
	class Visitor : public ee::RefVisitor<n3::INode>
	{
	public:
		Visitor(StageCanvas& canvas, const sm::vec2& start, const sm::vec2& end)
			: m_canvas(canvas), m_start(start), m_end(end) {}
		virtual void Visit(const n3::NodePtr& node, bool& next) override;

	private:
		StageCanvas& m_canvas;

		sm::vec2 m_start, m_end;

	}; // Visitor

private:
	StageCanvas& m_canvas;
	const NodeSelection& m_selection;

	sm::vec2 m_last_pos;

}; // SprRotateState

}