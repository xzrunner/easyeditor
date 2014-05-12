#include "EditWholeSkeletonAtomicOP.h"

namespace eanim
{
namespace eanim::edit_whole_skeleton
{

RotateBoneAOP::RotateBoneAOP(d2d::EditPanel* editPanel, WholeSkeleton::Sprite* selected, float delta)
{
	m_editPanel = editPanel;
	m_selected = selected;
	m_delta = delta;
}

void RotateBoneAOP::undo()
{
	m_selected->m_relativeCoords.delta -= m_delta;
	m_selected->computePosterityAbsoluteCoords();

	m_editPanel->Refresh();
}

void RotateBoneAOP::redo()
{
	m_selected->m_relativeCoords.delta += m_delta;
	m_selected->computePosterityAbsoluteCoords();

	m_editPanel->Refresh();
}

} // eanim::edit_whole_skeleton