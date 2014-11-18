#include "AutoCutOP.h"

namespace eimage
{

AutoCutOP::AutoCutOP(d2d::EditPanel* stage)
	: d2d::ZoomViewOP(stage, true)
{
}

bool AutoCutOP::onDraw() const
{
	if (d2d::ZoomViewOP::onDraw()) {
		return true;
	}

	d2d::PrimitiveDraw::drawPolyline(m_bound, d2d::LIGHT_RED, true);

	return false;
}

}