#include "SelectSpritesOP.h"
#include "StageCanvas.h"

namespace eshader
{

SelectSpritesOP::SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
	d2d::PropertySettingPanel* propertyPanel, d2d::AbstractEditCMPT* callback/* = NULL*/)
	: d2d::SelectSpritesOP(editPanel, spritesImpl, propertyPanel, callback)
{
}

bool SelectSpritesOP::onMouseLeftDown(int x, int y)
{
	if (d2d::SelectSpritesOP::onMouseLeftDown(x, y)) {
		return true;
	}

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	static_cast<StageCanvas*>(m_editPanel->getCanvas())->OnMousePressed(pos);

	return false;
}

}