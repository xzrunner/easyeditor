
#include "SelectBodyOP.h"
#include "BodyPropertySetting.h"
#include "WorldPropertySetting.h"
#include "DrawUtils.h"
#include "StagePanel.h"
#include "BodyEditStage.h"
#include "Context.h"

using namespace emodeling;

SelectBodyOP::SelectBodyOP(d2d::EditPanel* editPanel, 
						   d2d::MultiSpritesImpl* spritesImpl, 
						   d2d::PropertySettingPanel* propertyPanel, 
						   d2d::AbstractEditCMPT* callback/* = NULL*/,
						   const d2d::Colorf& color /*= d2d::Colorf(1, 1, 1)*/)
	: d2d::SelectSpritesOP(editPanel, spritesImpl, propertyPanel, callback, color)
	, m_mouseOn(NULL)
{
}

bool SelectBodyOP::onKeyDown(int keyCode)
{
	if (d2d::SelectSpritesOP::onKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE && m_mouseOn)
		m_mouseOn = NULL;

	return false;
}

bool SelectBodyOP::onMouseMove(int x, int y)
{
	if (d2d::SelectSpritesOP::onMouseMove(x, y)) return true;

	m_mouseOn = NULL;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	d2d::ISprite* selected = m_spritesImpl->querySpriteByPos(pos);
	if (selected)
		m_mouseOn = static_cast<libmodeling::Body*>(selected->getUserData());

	m_editPanel->Refresh();

	return false;
}

bool SelectBodyOP::onMouseLeftDClick(int x, int y)
{
	if (d2d::SelectSpritesOP::onMouseLeftDClick(x, y)) return true;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	d2d::ISprite* selected = m_spritesImpl->querySpriteByPos(pos);
	if (selected)
	{
		libshape::MainDialog<BodyEditStage> dlg(Context::Instance()->stage, selected);
		dlg.ShowModal();
		Context::Instance()->stage->resetCanvas();
	}

	return false;
}

bool SelectBodyOP::onDraw() const
{
	if (d2d::DrawRectangleOP::onDraw()) return true;

	m_selection->traverse(DrawSelectedVisitor());

	if (m_mouseOn)
		DrawUtils::drawBody(m_mouseOn, DrawUtils::e_mouseOn);

	return false;
}

bool SelectBodyOP::clear()
{
	if (d2d::SelectSpritesOP::clear()) return true;

	m_mouseOn = NULL;

	return false;
}

d2d::IPropertySetting* SelectBodyOP::createPropertySetting(d2d::ISprite* sprite) const
{
	if (sprite)
		return new BodyPropertySetting(m_editPanel, sprite);
	else
		return new WorldPropertySetting(m_editPanel);
}

//////////////////////////////////////////////////////////////////////////
// class SelectBodyOP::DrawSelectedVisitor
//////////////////////////////////////////////////////////////////////////

void SelectBodyOP::DrawSelectedVisitor::
visit(d2d::Object* object, bool& bFetchNext) 
{
	std::vector<d2d::Vector> bound;
	d2d::ISprite* sprite = static_cast<d2d::ISprite*>(object);
	libmodeling::Body* body = static_cast<libmodeling::Body*>(sprite->getUserData());
	DrawUtils::drawBody(body, DrawUtils::e_selected);
	bFetchNext = true;
}
