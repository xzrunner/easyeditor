#include "CrossGuides.h"
#include "std_functor.h"
#include "EE_RVG.h"
#include "color_config.h"

#include <SM_Calc.h>

#include <algorithm>

namespace ee
{

CrossGuides::CrossGuides()
	: m_selected(NULL)
{
}

CrossGuides::~CrossGuides()
{
	for_each(m_crosses.begin(), m_crosses.end(), DeletePointerFunctor<Cross>());
}

void CrossGuides::Add()
{
	m_crosses.push_back(new Cross());
}

void CrossGuides::Remove()
{
	if (m_crosses.empty())
		return;

	Cross* del = m_crosses.back();
	m_crosses.pop_back();
	if (m_selected == del)
	{
		if (m_crosses.empty()) {
			m_selected = NULL;
		} else {
			m_selected = m_crosses.back();
		}
	}
	delete del;
}

bool CrossGuides::OnMouseDown(const sm::vec2& pos)
{
	for (int i = 0, n = m_crosses.size(); i < n; ++i) {
		if (m_crosses[i]->Contain(pos)) {
			m_selected = m_crosses[i];
			return true;
		}
	}
	return false;
}

bool CrossGuides::OnMouseUp(const sm::vec2& pos)
{
	if (m_selected) {
		m_selected = NULL;
		return true;
	} else {
		return false;
	}
}

bool CrossGuides::OnMouseDrag(const sm::vec2& pos)
{
	if (m_selected) {
		m_selected->pos = pos;
		return true;
	} else {
		return false;
	}
}

void CrossGuides::Draw() const
{
	for (int i = 0, n = m_crosses.size(); i < n; ++i) {
		m_crosses[i]->Draw();
	}
}

//////////////////////////////////////////////////////////////////////////
// class CrossGuides::Cross
//////////////////////////////////////////////////////////////////////////

CrossGuides::Cross::Cross()
	: pos(0, 0)
{
}

void CrossGuides::Cross::Draw() const
{
	RVG::Color(LIGHT_RED);
	RVG::LineWidth(2);

	RVG::Circle(pos, RADIUS, false);
	RVG::Cross(pos, LENGTH);
}

bool CrossGuides::Cross::Contain(const sm::vec2& p) const
{
	return sm::dis_pos_to_pos(pos, p) < RADIUS;
}

}