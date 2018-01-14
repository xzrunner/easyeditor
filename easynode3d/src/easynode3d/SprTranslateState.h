#pragma once

#include "EditOpState.h"

#include <ee/Visitor.h>
#include <ee/Sprite.h>

#include <node3/INode.h>

namespace enode3d
{

class StageCanvas;
class NodeSelection;

class SprTranslateState : public EditOpState
{
public:
	SprTranslateState(StageCanvas& canvas, const NodeSelection& selection);

	virtual void OnMousePress(const sm::vec2& pos) override;
	virtual void OnMouseRelease(const sm::vec2& pos) override;
	virtual void OnMouseDrag(const sm::vec2& pos) override;

private:
	void Translate(const sm::vec2& first, const sm::vec2& curr);

private:
	class Visitor : public ee::RefVisitor<n3::INode>
	{
	public:
		Visitor(StageCanvas& canvas, const sm::vec2& last, const sm::vec2& curr)
			: m_canvas(canvas), m_last(last), m_curr(curr) {}
		virtual void Visit(const n3::NodePtr& node, bool& next) override;

	private:
		StageCanvas& m_canvas;

		sm::vec2 m_last, m_curr;

	}; // Visitor

private:
	StageCanvas& m_canvas;
	const NodeSelection& m_selection;

	sm::vec2 m_first_pos, m_last_pos;

}; // SprTranslateState

}
