#pragma once

#include "data/SceneNode.h"

#include <ee/Visitor.h>
#include <ee/SelectionSet.h>

#include <easynode3d/EditOpState.h>

namespace eone
{

class StageCanvas;
class SubjectMgr;

class NodeRotateState : public enode3d::EditOpState
{
public:
	NodeRotateState(StageCanvas& canvas, SubjectMgr& sub_mgr,
		const ee::SelectionSet<SceneNode>& selection);

	virtual void OnMousePress(const sm::vec2& pos) override;
	virtual void OnMouseRelease(const sm::vec2& pos) override;
	virtual void OnMouseDrag(const sm::vec2& pos) override;

private:
	void Rotate(const sm::vec2& start, const sm::vec2& end);

private:
	class Visitor : public ee::RefVisitor<SceneNode>
	{
	public:
		Visitor(StageCanvas& canvas, const sm::vec2& start, const sm::vec2& end)
			: m_canvas(canvas), m_start(start), m_end(end) {}
		virtual void Visit(const SceneNodePtr& node, bool& next) override;

	private:
		StageCanvas& m_canvas;

		sm::vec2 m_start, m_end;

	}; // Visitor

private:
	StageCanvas& m_canvas;
	SubjectMgr&  m_sub_mgr;

	const ee::SelectionSet<SceneNode>& m_selection;

	sm::vec2 m_last_pos;

}; // NodeRotateState

}