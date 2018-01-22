#pragma once

#include "editop/EditOpState.h"
#include "data/SceneNode.h"
#include "msg/SubjectMgr.h"

#include <ee/Visitor.h>
#include <ee/SelectionSet.h>

namespace eone
{

class StageCanvas;

class NodeTranslateState : public EditOpState
{
public:
	NodeTranslateState(StageCanvas& canvas, SubjectMgr& sub_mgr,
		const ee::SelectionSet<SceneNode>& selection);

	virtual void OnMousePress(const sm::vec2& pos) override;
	virtual void OnMouseRelease(const sm::vec2& pos) override;
	virtual void OnMouseDrag(const sm::vec2& pos) override;

private:
	void Translate(const sm::vec2& first, const sm::vec2& curr);

private:
	class Visitor : public ee::RefVisitor<SceneNode>
	{
	public:
		Visitor(StageCanvas& canvas, const sm::vec2& last, const sm::vec2& curr)
			: m_canvas(canvas), m_last(last), m_curr(curr) {}
		virtual void Visit(const SceneNodePtr& node, bool& next) override;

	private:
		StageCanvas& m_canvas;

		sm::vec2 m_last, m_curr;

	}; // Visitor

private:
	StageCanvas& m_canvas;
	SubjectMgr&  m_sub_mgr;

	const ee::SelectionSet<SceneNode>& m_selection;

	sm::vec2 m_first_pos, m_last_pos;

}; // NodeTranslateState

}
