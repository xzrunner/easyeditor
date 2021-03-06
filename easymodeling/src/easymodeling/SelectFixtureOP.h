#ifndef _EASYMODELING_SELECT_FIXTURE_OP_H_
#define _EASYMODELING_SELECT_FIXTURE_OP_H_

#include <ee/DrawRectangleOP.h>

namespace ee { class PropertySettingPanel; }

namespace emodeling
{

class Fixture;
class StagePanel;

class SelectFixtureOP : public ee::DrawRectangleOP
{
public:
	SelectFixtureOP(StagePanel* editPanel, ee::PropertySettingPanel* property);
	virtual ~SelectFixtureOP();

	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseMove(int x, int y) override;

	virtual bool OnDraw() const override;
	virtual bool Clear() override;

private:
	StagePanel* m_stagePanel;

	ee::PropertySettingPanel* m_property;

	sm::vec2 m_first_pos;

	Fixture* m_selected;
	Fixture* m_mouseOn;

}; // SelectFixtureOP

}

#endif // _EASYMODELING_SELECT_FIXTURE_OP_H_
