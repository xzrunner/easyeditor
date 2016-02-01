
#ifndef EMODELING_SELECT_FIXTURE_OP_H
#define EMODELING_SELECT_FIXTURE_OP_H



namespace libmodeling { class Fixture; }

namespace emodeling
{
	class StagePanel;

	class SelectFixtureOP : public ee::DrawRectangleOP
	{
	public:
		SelectFixtureOP(StagePanel* editPanel, ee::PropertySettingPanel* property);
		virtual ~SelectFixtureOP();

		virtual bool OnMouseLeftDown(int x, int y);
		virtual bool OnMouseLeftUp(int x, int y);
		virtual bool OnMouseMove(int x, int y);

		virtual bool OnDraw() const;
		virtual bool Clear();

	private:
		StagePanel* m_stagePanel;

		ee::PropertySettingPanel* m_property;

		ee::Vector m_first_pos;

		libmodeling::Fixture* m_selected;
		libmodeling::Fixture* m_mouseOn;

	}; // SelectFixtureOP
}

#endif // EMODELING_SELECT_FIXTURE_OP_H
