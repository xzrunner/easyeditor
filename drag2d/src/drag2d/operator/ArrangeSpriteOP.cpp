#ifndef D2D_ARRANGE_SPRITE_OP_CPP_
#define D2D_ARRANGE_SPRITE_OP_CPP_

#include "ArrangeSpriteOP.h"
#include "ArrangeSpriteImpl.h"

#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "dataset/AbstractBV.h"
#include "common/Math.h"
#include "common/visitors.h"
#include "common/Matrix.h"
#include "common/Settings.h"
#include "view/EditPanel.h"
#include "view/MultiSpritesImpl.h"
#include "view/PropertySettingPanel.h"
#include "history/ArrangeSpriteAtomicOP.h"
#include "render/PrimitiveDraw.h"

namespace d2d
{

template <typename TBase>
const float ArrangeSpriteOP<TBase>::SCALE_NODE_RADIUS = 2.0f;

template <typename TBase>
ArrangeSpriteOP<TBase>::ArrangeSpriteOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
	PropertySettingPanel* propertyPanel/* = NULL*/, AbstractEditCMPT* callback/* = NULL*/, 
	bool isDeformOpen/* = true*/, bool isAutoAlignOpen /*= true*/)
	: TBase(editPanel, spritesImpl, propertyPanel, callback)
	, m_isDeformOpen(isDeformOpen)
	, m_spritesImpl(spritesImpl)
	, m_propertyPanel(propertyPanel)
	, m_bRightPress(false)
	, m_selected(NULL)
	, m_bDirty(false)
{
	m_selection = spritesImpl->getSpriteSelection();
	m_selection->retain();

	m_impl = new ArrangeSpriteImpl(spritesImpl);
	m_impl->SetAutoAlignOpen(isAutoAlignOpen);

	m_firstPos.setInvalid();

	m_ctrlNodeSelected.setInvalid();

	m_shearNodeStyle.color.set(0.2f, 0.8f, 0.2f);
	m_shearNodeStyle.fill = false;
	m_shearNodeStyle.size = 2;
}

template <typename TBase>
ArrangeSpriteOP<TBase>::~ArrangeSpriteOP()
{
	m_selection->release();
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::onKeyDown(int keyCode)
{
	std::vector<ISprite*> sprites;
	m_selection->traverse(FetchAllVisitor<ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		sprites[i]->retain();

	if (TBase::onKeyDown(keyCode)) 
	{
		for (size_t i = 0, n = sprites.size(); i < n; ++i)
			sprites[i]->release();
		return true;
	}

	switch (keyCode)
	{
	case WXK_DELETE:
		{
			std::vector<ISprite*> noPhysicsSprites;
			for (size_t i = 0, n = sprites.size(); i < n; ++i)
				if (!sprites[i]->getBody()) 
					noPhysicsSprites.push_back(sprites[i]);
			m_editPanel->addHistoryOP(new arrange_sprite::DeleteSpritesAOP(noPhysicsSprites, m_spritesImpl));
		}
		m_spritesImpl->removeSpriteSelection();
		m_selected = NULL;
		break;
	case 'a': case 'A':
		onDirectionKeyDown(e_left);
		break;
	case 'd': case 'D':
		onDirectionKeyDown(e_right);
		break;
	case 's': case 'S':
		onDirectionKeyDown(e_down);
		break;
	case 'w': case 'W':
		onDirectionKeyDown(e_up);
		break;
	case WXK_SPACE:
		onSpaceKeyDown();
		break;
	}

	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		sprites[i]->release();
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::onKeyUp(int keyCode)
{
	if (TBase::onKeyUp(keyCode)) 
		return true;

	if (m_propertyPanel && m_bDirty)
	{
		m_propertyPanel->enablePropertyGrid(true);
		m_propertyPanel->updatePropertyGrid();
		m_bDirty = false;
	}

	return false;
}

// todo: ugly
template <typename TBase>
bool ArrangeSpriteOP<TBase>::onMouseLeftDown(int x, int y)
{
	m_lastPos = m_editPanel->transPosScreenToProject(x, y);
	m_ctrlNodeSelected.setInvalid();

	if (m_isDeformOpen && m_selected)
	{
		if (isOffsetEnable())
		{
			d2d::Vector offset = m_selected->getPosition() + m_selected->getOffset();
			if (Math::getDistance(offset, m_lastPos) < SCALE_NODE_RADIUS)
			{
				m_selOffset = true;
				return false;
			}
		}

		Vector ctrlNodes[8];
		m_impl->GetSpriteCtrlNodes(m_selected, ctrlNodes);
		for (int i = 0; i < 8; ++i)
		{
			if (Math::getDistance(ctrlNodes[i], m_lastPos) < SCALE_NODE_RADIUS)
			{
				m_ctrlNodeSelected.pos = ctrlNodes[i];
				m_ctrlNodeSelected.type = CtrlNodeType(i);
				return false;
			}
		}
	}

	if (TBase::onMouseLeftDown(x, y)) return true;

	setScalingFromSelected();

	if (!m_selection->empty())
	{
		m_firstPos = m_lastPos;
	}
	m_bRightPress = false;

	m_impl->SetAutoAlignInvisible();

	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::onMouseLeftUp(int x, int y)
{
	if (TBase::onMouseLeftUp(x, y)) return true;

	setScalingFromSelected();

	if (isOffsetEnable())
		m_selOffset = false;

	if (m_firstPos.isValid() && !m_selection->empty() && !m_bRightPress)
	{
		Vector pos = m_editPanel->transPosScreenToProject(x, y);
		if (pos != m_firstPos)
		{
			m_editPanel->addHistoryOP(new arrange_sprite::MoveSpritesAOP(*m_selection, pos - m_firstPos));
			m_firstPos.setInvalid();
		}
	}

	if (m_propertyPanel && m_bDirty)
	{
		m_propertyPanel->enablePropertyGrid(true);
		m_propertyPanel->updatePropertyGrid();
		m_bDirty = false;
	}

	if (Settings::bSpriteCapture && m_impl->IsAutoAlignOpen() && !m_selection->empty())
	{
		std::vector<ISprite*> sprites;
		m_selection->traverse(FetchAllVisitor<ISprite>(sprites));
		m_impl->Align(sprites);
	}

	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::onMouseRightDown(int x, int y)
{
	m_lastPos = m_editPanel->transPosScreenToProject(x, y);
	m_firstPos = m_lastPos;
	if (m_selection->size() == 1)
		m_bRightPress = true;

	if (TBase::onMouseRightDown(x, y)) return true;

	m_ctrlNodeSelected.setInvalid();
	if (m_isDeformOpen && m_selected)
	{
		Vector ctrlNodes[8];
		m_impl->GetSpriteCtrlNodes(m_selected, ctrlNodes);
		for (int i = 0; i < 8; ++i)
		{
			if (Math::getDistance(ctrlNodes[i], m_lastPos) < SCALE_NODE_RADIUS)
			{
				m_ctrlNodeSelected.pos = ctrlNodes[i];
				m_ctrlNodeSelected.type = CtrlNodeType(i);
				return false;
			}
		}
	}

	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::onMouseRightUp(int x, int y)
{
	if (TBase::onMouseRightUp(x, y)) return true;

	if (m_firstPos.isValid() && !m_selection->empty())
	{
		Vector pos = m_editPanel->transPosScreenToProject(x, y);
		d2d::ISprite* sprite = m_spritesImpl->querySpriteByPos(pos);
		if (pos == m_firstPos && sprite)
		{
			wxMenu menu;
			setRightPopupMenu(menu);
			m_editPanel->PopupMenu(&menu, x, y);
		}
		else
		{
			m_editPanel->addHistoryOP(new arrange_sprite::RotateSpritesAOP(*m_selection, m_firstPos, pos));
		}
	}

	if (m_propertyPanel && m_bDirty)
	{
		m_propertyPanel->enablePropertyGrid(true);
		m_propertyPanel->updatePropertyGrid();
		m_bDirty = false;
	}

	m_bRightPress = false;

	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::onMouseDrag(int x, int y)
{
	if (TBase::onMouseDrag(x, y)) return true;

	if (m_isDeformOpen && m_selected)
	{
		Vector pos = m_editPanel->transPosScreenToProject(x, y);
		if (isOffsetEnable() && m_selOffset)
		{
			d2d::Vector offset = Math::rotateVector(pos - m_selected->getCenter(), -m_selected->getAngle());
			m_selected->setOffset(offset);
			m_editPanel->Refresh();
			return false;
		}
		else if (m_ctrlNodeSelected.isValid())
		{
// 				if (m_ctrlNodeSelected.type < UP)
// 					scaleSprite(pos);
// 				else
// 					shearSprite(pos);

			if (m_bRightPress && m_ctrlNodeSelected.type >= UP)
				shearSprite(pos);
			else
				scaleSprite(pos);
			return false;
		}
	}

	if (m_selection->empty()) return false;

	Vector pos = m_editPanel->transPosScreenToProject(x, y);
	if (m_bRightPress)
		rotateSprite(pos);
	else
		translateSprite(pos - m_lastPos);
	m_lastPos = pos;

	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::onPopMenuSelected(int type)
{
	if (TBase::onPopMenuSelected(type)) return true;

	switch (type)
	{
	case EditPanel::Menu_UpOneLayer:
		{
			std::vector<d2d::ISprite*> selected;
			m_selection->traverse(d2d::FetchAllVisitor<d2d::ISprite>(selected));
			for (size_t i = 0, n = selected.size(); i < n; ++i)
				m_spritesImpl->resetSpriteOrder(selected[i], true);
		}
		break;
	case EditPanel::Menu_DownOneLayer:
		{
			std::vector<d2d::ISprite*> selected;
			m_selection->traverse(d2d::FetchAllVisitor<d2d::ISprite>(selected));
			for (size_t i = 0, n = selected.size(); i < n; ++i)
				m_spritesImpl->resetSpriteOrder(selected[i], false);
		}
		break;
	}

	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::onDraw() const
{
	if (TBase::onDraw()) return true;

	if (m_isDeformOpen && m_selected)
	{
		Vector ctrlNodes[8];
		m_impl->GetSpriteCtrlNodes(m_selected, ctrlNodes);
		for (int i = 0; i < 4; ++i)
			PrimitiveDraw::drawCircle(ctrlNodes[i], SCALE_NODE_RADIUS, false, 2, Colorf(0.2f, 0.8f, 0.2f));
		for (int i = 4; i < 8; ++i)
			PrimitiveDraw::drawCircle(ctrlNodes[i], SCALE_NODE_RADIUS, true, 2, Colorf(0.2f, 0.8f, 0.2f));

		if (isOffsetEnable())
		{
			d2d::Vector offset = m_selected->getPosition() + m_selected->getOffset();
			PrimitiveDraw::drawCircle(offset, SCALE_NODE_RADIUS, true, 2, Colorf(0.8f, 0.2f, 0.2f));
		}
	}

	m_impl->OnDraw();

	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::clear()
{
	if (TBase::clear()) return true;

	m_lastPos.setInvalid();
	m_bRightPress = false;
	m_selected = NULL;

	return false;
}

template <typename TBase>
void ArrangeSpriteOP<TBase>::onDirectionKeyDown(DirectionType type)
{
	switch (type)
	{
	case e_left:
		translateSprite(Vector(-1, 0));
		break;
	case e_right:
		translateSprite(Vector(1, 0));
		break;
	case e_down:
		translateSprite(Vector(0, -1));
		break;
	case e_up:
		translateSprite(Vector(0, 1));
		break;
	}
}

template <typename TBase>
void ArrangeSpriteOP<TBase>::onSpaceKeyDown()
{
	std::vector<ISprite*> selected;
	m_selection->traverse(FetchAllVisitor<ISprite>(selected));
	for (size_t i = 0, n = selected.size(); i < n; ++i)
	{
		selected[i]->setTransform(Vector(0, 0), 0);
		//selected[i]->setOffset(Vector(0, 0));
	}
	m_editPanel->Refresh();
}

//template <typename TBase>
//void ArrangeSpriteOP<TBase>::translateSprite(const Vector& delta)
//{
//	m_selection->traverse(TranslateVisitor(delta));
//	if (!m_selection->empty()) 
//	{
//		if (m_propertyPanel && !m_bDirty)
//		{
//			m_propertyPanel->enablePropertyGrid(false);
//			m_bDirty = true;
//		}
//		m_editPanel->Refresh();
//	}
//}
//
//template <typename TBase>
//void ArrangeSpriteOP<TBase>::rotateSprite(const Vector& dst)
//{
//	if (m_selection->size() != 1) return;
//	m_selection->traverse(RotateVisitor(m_lastPos, dst));
//	if (m_propertyPanel && !m_bDirty)
//	{
//		m_propertyPanel->enablePropertyGrid(false);
//		m_bDirty = true;
//	}
//	m_editPanel->Refresh();
//}
//
//template <typename TBase>
//void ArrangeSpriteOP<TBase>::scaleSprite(const Vector& currPos)
//{
//	if (!m_isDeformOpen) return;
//
//	float hw = m_selected->getSymbol().getSize().xLength() * 0.5f;
//	float hh = m_selected->getSymbol().getSize().yLength() * 0.5f;
//	love::Matrix t;
//	const Vector& center = m_selected->getCenter();
//	t.setTransformation(center.x, center.y, m_selected->getAngle(),
//		m_selected->getScale().x, m_selected->getScale().y, 0, 0, m_selected->getShear().x, m_selected->getShear().y);
//
//	Vector ori, fix;
//	if (m_ctrlNodeSelected.type == UP)
//		ori = Math::transVector(Vector(0.0f, hh), t);
//	else if (m_ctrlNodeSelected.type == DOWN)
//		ori = Math::transVector(Vector(0.0f, -hh), t);
//	else if (m_ctrlNodeSelected.type == LEFT)
//		ori = Math::transVector(Vector(-hw, 0.0f), t);
//	else if (m_ctrlNodeSelected.type == RIGHT)
//		ori = Math::transVector(Vector(hw, 0.0f), t);
//	else if (m_ctrlNodeSelected.type == LEFT_UP)
//		ori = Math::transVector(Vector(-hw, hh), t);
//	else if (m_ctrlNodeSelected.type == RIGHT_UP)
//		ori = Math::transVector(Vector(hw, hh), t);
//	else if (m_ctrlNodeSelected.type == LEFT_DOWN)
//		ori = Math::transVector(Vector(-hw, -hh), t);
//	else if (m_ctrlNodeSelected.type == RIGHT_DOWN)
//		ori = Math::transVector(Vector(hw, -hh), t);
//	Math::getFootOfPerpendicular(center, ori, currPos, &fix);
//
//	float scale = Math::getDistance(center, fix) / Math::getDistance(center, ori);
//	scale += (1 - scale) * 0.5f;
//	if (m_ctrlNodeSelected.type == UP || m_ctrlNodeSelected.type == DOWN)
//		m_selected->setScale(m_selected->getScale().x, scale * m_selected->getScale().y);
//	else if (m_ctrlNodeSelected.type == LEFT || m_ctrlNodeSelected.type == RIGHT)
//		m_selected->setScale(scale * m_selected->getScale().x, m_selected->getScale().y);
//	else		
//		m_selected->setScale(scale * m_selected->getScale().x, scale * m_selected->getScale().y);
//
//	Vector offset = (fix - ori) * 0.5f;
//	m_selected->translate(offset);
//
//	if (m_propertyPanel && !m_bDirty)
//	{
//		m_propertyPanel->enablePropertyGrid(false);
//		m_bDirty = true;
//	}
//	m_editPanel->Refresh();
//}
//
//template <typename TBase>
//void ArrangeSpriteOP<TBase>::shearSprite(const Vector& currPos)
//{
//	if (!m_isDeformOpen) return;
//
//	// fix pos
//	Vector pos;
//	Vector ctrls[8];
//	m_impl->GetSpriteCtrlNodes(m_selected, ctrls);
//	if (m_ctrlNodeSelected.type == UP)
//		Math::getFootOfPerpendicular(ctrls[0], ctrls[1], currPos, &pos);
//	else if (m_ctrlNodeSelected.type == DOWN)
//		Math::getFootOfPerpendicular(ctrls[2], ctrls[3], currPos, &pos);
//	else if (m_ctrlNodeSelected.type == LEFT)
//		Math::getFootOfPerpendicular(ctrls[0], ctrls[2], currPos, &pos);
//	else if (m_ctrlNodeSelected.type == RIGHT)
//		Math::getFootOfPerpendicular(ctrls[1], ctrls[3], currPos, &pos);
//
//	// M * p = p'
//	//
//	// x' = e0 * x + e4 * y + e12
//	// y' = e1 * x + e5 * y + e13
//	// 
//	// e0 = c * sx - ky * s * sy
//	// e4 = kx * c * sx - s * sy
//	// e12 = px
//	//
//	// pos.x = (c * sx - ky * s * sy) * x + (kx * c * sx - s * sy) * y + px
//	// pos.y = (s * sx + ky * c * sy) * x + (kx * s * sx + c * sy) * y + py
//	// pos.x = c*sx*x - ky*s*sy*x + kx*c*sx*y - s*sy*y + px
//	// pos.y = s*sx*x + ky*c*sy*x + kx*s*sx*y + c*sy*y + py
//	// 
//	// kx = (pos.x - c*sx*x + ky*s*sy*x + s*sy*y - px) / (c*sx*y)
//	// kx = (pos.y - s*sx*x - ky*c*sy*x - c*sy*y - py) / (s*sx*y)
//	// ky = (pos.x - c*sx*x - kx*c*sx*y + s*sy*y - px) / (-s*sy*x)
//	// ky = (pos.y - s*sx*x - kx*s*sx*y - c*sy*y - py) / (c*sy*x)
//	float c = cos(m_selected->getAngle()), s = sin(m_selected->getAngle());
//	float sx = m_selected->getScale().x, sy = m_selected->getScale().y;
//	float px = m_selected->getPosition().x, py = m_selected->getPosition().y;
//	float kx = m_selected->getShear().x,
//		ky = m_selected->getShear().y;
//
//	float x, y;
//	float hw = m_selected->getSymbol().getSize().xLength() * 0.5f,
//		hh = m_selected->getSymbol().getSize().yLength() * 0.5f;
//
//	if (m_ctrlNodeSelected.type == UP)
//	{
//		x = 0; y = hh;
//		if (c != 0)
//			kx = (pos.x - c*sx*x + ky*s*sy*x + s*sy*y - px) / (c*sx*y);
//		else
//			kx = (pos.y - s*sx*x - ky*c*sy*x - c*sy*y - py) / (s*sx*y);
//	}
//	else if (m_ctrlNodeSelected.type == DOWN)
//	{
//		x = 0; y = -hh;
//		if (c != 0)
//			kx = (pos.x - c*sx*x + ky*s*sy*x + s*sy*y - px) / (c*sx*y);
//		else
//			kx = (pos.y - s*sx*x - ky*c*sy*x - c*sy*y - py) / (s*sx*y);
//	}
//	else if (m_ctrlNodeSelected.type == LEFT)
//	{
//		x = -hw; y = 0;
//		if (s != 0)
//			ky = (pos.x - c*sx*x - kx*c*sx*y + s*sy*y - px) / (-s*sy*x);
//		else
//			ky = (pos.y - s*sx*x - kx*s*sx*y - c*sy*y - py) / (c*sy*x);
//	}
//	else if (m_ctrlNodeSelected.type == RIGHT)
//	{
//		x = hw; y = 0;
//		if (s != 0)
//			ky = (pos.x - c*sx*x - kx*c*sx*y + s*sy*y - px) / (-s*sy*x);
//		else
//			ky = (pos.y - s*sx*x - kx*s*sx*y - c*sy*y - py) / (c*sy*x);
//	}
//
//	m_selected->setShear(kx, ky);
//
//	if (m_propertyPanel && !m_bDirty)
//	{
//		m_propertyPanel->enablePropertyGrid(false);
//		m_bDirty = true;
//	}
//	m_editPanel->Refresh();
//}

template <typename TBase>
void ArrangeSpriteOP<TBase>::setRightPopupMenu(wxMenu& menu)
{
	menu.Append(EditPanel::Menu_UpOneLayer, EditPanel::menu_entries[EditPanel::Menu_UpOneLayer]);
	menu.Append(EditPanel::Menu_DownOneLayer, EditPanel::menu_entries[EditPanel::Menu_DownOneLayer]);
}

template <typename TBase>
void ArrangeSpriteOP<TBase>::setScalingFromSelected()
{
	if (!m_isDeformOpen) return;

	bool refresh = false;
	if (m_selection->size() == 1)
	{
		refresh = m_selected == NULL;
		std::vector<ISprite*> sprites;
		m_selection->traverse(FetchAllVisitor<ISprite>(sprites));
		m_selected = sprites[0];
	}
	else
	{
		refresh = m_selected != NULL;
		m_selected = NULL;
	}

	if (refresh) 
		m_editPanel->Refresh();
}

////////////////////////////////////////////////////////////////////////////
//// class ArrangeSpriteOP<TBase>::TranslateVisitor
////////////////////////////////////////////////////////////////////////////
//
//template <typename TBase>
//ArrangeSpriteOP<TBase>::TranslateVisitor::
//	TranslateVisitor(const Vector& delta)
//	: m_delta(delta)
//{
//}
//
//template <typename TBase>
//void ArrangeSpriteOP<TBase>::TranslateVisitor::
//	visit(Object* object, bool& bFetchNext)
//{
//	ISprite* sprite = static_cast<ISprite*>(object);
//	sprite->translate(m_delta);
//	bFetchNext = true;
//}
//
////////////////////////////////////////////////////////////////////////////
//// class ArrangeSpriteOP<TBase>::RotateVisitor
////////////////////////////////////////////////////////////////////////////
//
//template <typename TBase>
//ArrangeSpriteOP<TBase>::RotateVisitor::
//	RotateVisitor(const Vector& start, const Vector& end)
//	: m_start(start), m_end(end)
//{
//}
//
//template <typename TBase>
//void ArrangeSpriteOP<TBase>::RotateVisitor::
//	visit(Object* object, bool& bFetchNext)
//{
//	ISprite* sprite = static_cast<ISprite*>(object);
//
//	Vector center = sprite->getPosition() + sprite->getOffset();
//	float angle = Math::getAngleInDirection(center, m_start, m_end);
//	sprite->rotate(angle);
//
//	bFetchNext = false;
//}
//
//}

#endif // D2D_ARRANGE_SPRITE_OP_CPP_
