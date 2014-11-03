#ifndef _LIBSKETCH_ROTATE_SPRITE_STATE_H_
#define _LIBSKETCH_ROTATE_SPRITE_STATE_H_

#include "IEditState.h"

#include <easy3d.h>

namespace libsketch
{

class StagePanel;
class Sprite;

class RotateSpriteState : public IEditState
{
public:
	RotateSpriteState(StagePanel* stage,
		const d2d::SpriteSelection& selection);

	virtual void OnMousePress(const ivec2& pos);
	virtual void OnMouseRelease(const ivec2& pos);
	virtual void OnMouseMove(const ivec2& pos);	

private:
	void Rotate(const e3d::Camera& cam, const ivec2& start, const ivec2& end);

private:
	class Visitor : public d2d::IVisitor
	{
	public:
		Visitor(StagePanel* stage, const e3d::Camera& cam, const ivec2& start, const ivec2& end) 
			: m_stage(stage), m_cam(cam), m_start(start), m_end(end) {}
		virtual void visit(d2d::Object* object, bool& bFetchNext);

	private:
		StagePanel* m_stage;
		const e3d::Camera& m_cam;
		ivec2 m_start, m_end;

	}; // Visitor

private:
	StagePanel* m_stage;
	const d2d::SpriteSelection& m_selection;

	ivec2 m_last_pos;

}; // RotateSpriteState

}

#endif // _LIBSKETCH_ROTATE_SPRITE_STATE_H_