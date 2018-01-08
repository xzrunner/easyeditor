#pragma once

#include "EditOpState.h"

#include <ee/SpriteSelection.h>
#include <ee/Visitor.h>

namespace e3d
{

class StageCanvas;
class Sprite;

class SprRotateState : public EditOpState
{
public:
	SprRotateState(StageCanvas& canvas, const ee::SpriteSelection& selection);

	virtual void OnMousePress(const sm::vec2& pos) override;
	virtual void OnMouseRelease(const sm::vec2& pos) override;
	virtual void OnMouseMove(const sm::vec2& pos) override;	

private:
	void Rotate(const sm::vec2& start, const sm::vec2& end);

private:
	class Visitor : public ee::RefVisitor<ee::Sprite>
	{
	public:
		Visitor(StageCanvas& canvas, const sm::vec2& start, const sm::vec2& end)
			: m_canvas(canvas), m_start(start), m_end(end) {}
		virtual void Visit(const ee::SprPtr& spr, bool& next) override;

	private:
		StageCanvas& m_canvas;

		sm::vec2 m_start, m_end;

	}; // Visitor

private:
	StageCanvas& m_canvas;
	const ee::SpriteSelection& m_selection;

	sm::vec2 m_last_pos;

}; // SprRotateState

}