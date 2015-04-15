
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
						   d2d::AbstractEditCMPT* callback/* = NULL*/)
	: d2d::SelectSpritesOP(editPanel, spritesImpl, propertyPanel, callback)
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

	// todo

// 	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
// 	d2d::ISprite* selected = m_spritesImpl->querySpriteByPos(pos);
// 	if (selected)
// 	{
// 		libshape::Symbol symbol;
// 		symbol.SetBG(&const_cast<d2d::ISymbol&>(selected->getSymbol()));
// 		libshape::EditDialog<BodyEditStage> dlg(Context::Instance()->stage, &symbol);
// 		dlg.ShowModal();
// 		Context::Instance()->stage->resetCanvas();
// 	}

	return false;
}

bool SelectBodyOP::onDraw() const
{
	if (d2d::DrawRectangleOP::onDraw()) return true;

	m_selection->Traverse(DrawSelectedVisitor());

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
