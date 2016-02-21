#ifndef _EASYSKETCH_ROTATE_SPRITE_STATE_H_
#define _EASYSKETCH_ROTATE_SPRITE_STATE_H_

#include "IEditState.h"

#include <ee/SpriteSelection.h>
#include <ee/Visitor.h>

#include <easy3d.h>

namespace esketch
{

class StagePanel;
class Sprite;

class RotateSpriteState : public IEditState
{
public:
	RotateSpriteState(StagePanel* stage,
		const ee::SpriteSelection& selection);

	virtual void OnMousePress(const ee::ivec2& pos);
	virtual void OnMouseRelease(const ee::ivec2& pos);
	virtual void OnMouseMove(const ee::ivec2& pos);	

private:
	void Rotate(const e3d::Camera& cam, const ee::ivec2& start, const ee::ivec2& end);

private:
	class Visitor : public ee::Visitor
	{
	public:
		Visitor(StagePanel* stage, const e3d::Camera& cam, const ee::ivec2& start, const ee::ivec2& end) 
			: m_stage(stage), m_cam(cam), m_start(start), m_end(end) {}
		virtual void Visit(ee::Object* object, bool& next);

	private:
		StagePanel* m_stage;
		const e3d::Camera& m_cam;
		ee::ivec2 m_start, m_end;

	}; // Visitor

private:
	StagePanel* m_stage;
	const ee::SpriteSelection& m_selection;

	ee::ivec2 m_last_pos;

}; // RotateSpriteState

}

#endif // _EASYSKETCH_ROTATE_SPRITE_STATE_H_