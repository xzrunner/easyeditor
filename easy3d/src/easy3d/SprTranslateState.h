#pragma once

#include "EditOpState.h"

#include <ee/Visitor.h>
#include <ee/Sprite.h>

namespace ee { class SpriteSelection; }

namespace e3d
{

	class StageCanvas;

class SprTranslateState : public EditOpState
{
public:
	SprTranslateState(StageCanvas& canvas, const ee::SpriteSelection& selection);

	virtual void OnMousePress(const sm::vec2& pos) override;
	virtual void OnMouseRelease(const sm::vec2& pos) override;
	virtual void OnMouseMove(const sm::vec2& pos) override;	

private:
	void Translate(const sm::vec2& first, const sm::vec2& curr);

private:
	class Visitor : public ee::RefVisitor<ee::Sprite>
	{
	public:
		Visitor(StageCanvas& canvas, const sm::vec2& last, const sm::vec2& curr)
			: m_canvas(canvas), m_last(last), m_curr(curr) {}
		virtual void Visit(const ee::SprPtr& spr, bool& next) override;

	private:
		StageCanvas& m_canvas;

		sm::vec2 m_last, m_curr;

	}; // Visitor

private:
	StageCanvas& m_canvas;
	const ee::SpriteSelection& m_selection;

	sm::vec2 m_first_pos, m_last_pos;

}; // SprTranslateState

}
