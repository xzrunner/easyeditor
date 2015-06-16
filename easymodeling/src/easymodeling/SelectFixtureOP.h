
#ifndef EMODELING_SELECT_FIXTURE_OP_H
#define EMODELING_SELECT_FIXTURE_OP_H

#include <drag2d.h>

namespace libmodeling { class Fixture; }

namespace emodeling
{
	class StagePanel;

	class SelectFixtureOP : public d2d::DrawRectangleOP
	{
	public:
		SelectFixtureOP(StagePanel* editPanel, d2d::PropertySettingPanel* propertyPanel);
		virtual ~SelectFixtureOP();

		virtual bool OnMouseLeftDown(int x, int y);
		virtual bool OnMouseLeftUp(int x, int y);
		virtual bool OnMouseMove(int x, int y);

		virtual bool OnDraw() const;
		virtual bool Clear();

	private:
		StagePanel* m_stagePanel;

		d2d::PropertySettingPanel* m_propertyPanel;

		d2d::Vector m_firstPos;

		libmodeling::Fixture* m_selected;
		libmodeling::Fixture* m_mouseOn;

	}; // SelectFixtureOP
}

#endif // EMODELING_SELECT_FIXTURE_OP_H
