#pragma once

#include <ee/Visitor.h>
#include <ee/SelectionSet.h>

#include <node3/SceneNode.h>

#include <easynode3d/EditOpState.h>

namespace eone
{

class StageCanvas;
class SubjectMgr;

class NodeTranslateState : public enode3d::EditOpState
{
public:
	NodeTranslateState(StageCanvas& canvas, SubjectMgr& sub_mgr,
		const ee::SelectionSet<n3::SceneNode>& selection);

	virtual void OnMousePress(const sm::vec2& pos) override;
	virtual void OnMouseRelease(const sm::vec2& pos) override;
	virtual void OnMouseDrag(const sm::vec2& pos) override;

private:
	void Translate(const sm::vec2& first, const sm::vec2& curr);

private:
	class Visitor : public ee::RefVisitor<n3::SceneNode>
	{
	public:
		Visitor(StageCanvas& canvas, const sm::vec2& last, const sm::vec2& curr)
			: m_canvas(canvas), m_last(last), m_curr(curr) {}
		virtual void Visit(const n3::SceneNodePtr& node, bool& next) override;

	private:
		StageCanvas& m_canvas;

		sm::vec2 m_last, m_curr;

	}; // Visitor

private:
	StageCanvas& m_canvas;
	SubjectMgr&  m_sub_mgr;

	const ee::SelectionSet<n3::SceneNode>& m_selection;

	sm::vec2 m_first_pos, m_last_pos;

}; // NodeTranslateState

}
