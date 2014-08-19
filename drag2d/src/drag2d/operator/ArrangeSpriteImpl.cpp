#include "ArrangeSpriteImpl.h"

#include "TranslateSpriteState.h"
#include "RotateSpriteState.h"
#include "ScaleSpriteState.h"
#include "ShearSpriteState.h"
#include "OffsetSpriteState.h"

#include "common/Math.h"
#include "common/Settings.h"
#include "view/EditPanel.h"
#include "view/MultiSpritesImpl.h"
#include "view/PropertySettingPanel.h"
#include "history/DeleteSpriteAOP.h"
#include "render/PrimitiveDraw.h"

namespace d2d
{

const float ArrangeSpriteImpl::SCALE_NODE_RADIUS = 2.0f;

ArrangeSpriteImpl::ArrangeSpriteImpl(EditPanel* editPanel,
									 MultiSpritesImpl* spritesImpl,
									 PropertySettingPanel* propertyPanel,
									 bool isDeformOpen,
									 bool isAutoAlignOpen,
									 bool isOffsetOpen) 
	: m_editPanel(editPanel)
	, m_spritesImpl(spritesImpl)
	, m_propertyPanel(propertyPanel)
	, m_isDeformOpen(isDeformOpen)
	, m_align(spritesImpl)
	, m_op_state(NULL)
	, m_is_offset_open(isOffsetOpen)
{
	m_align.SetOpen(isAutoAlignOpen);

	m_selection = spritesImpl->getSpriteSelection();
	m_selection->retain();

	m_right_down_pos.setInvalid();
}

ArrangeSpriteImpl::~ArrangeSpriteImpl()
{
	m_selection->release();
	delete m_op_state;
}

void ArrangeSpriteImpl::onKeyDown(int keyCode)
{
	switch (keyCode)
	{
	case WXK_DELETE:
		// add history
		{
			std::vector<ISprite*> sprites;
			m_selection->traverse(FetchAllVisitor<ISprite>(sprites));

			std::vector<ISprite*> noPhysicsSprites;
			for (size_t i = 0, n = sprites.size(); i < n; ++i)
				if (!sprites[i]->getBody()) 
					noPhysicsSprites.push_back(sprites[i]);
			m_editPanel->addHistoryOP(new DeleteSpriteAOP(noPhysicsSprites, m_spritesImpl));
		}
		m_spritesImpl->removeSpriteSelection();
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
}

void ArrangeSpriteImpl::onKeyUp(int keyCode)
{
	if (m_propertyPanel)
	{
		m_propertyPanel->enablePropertyGrid(true);
		m_propertyPanel->updatePropertyGrid();
	}
}

void ArrangeSpriteImpl::onMouseLeftDown(int x, int y)
{
	m_align.SetInvisible();

	ISprite* selected = NULL;
	if (m_selection->size() == 1)
	{
		std::vector<ISprite*> sprites;
		m_selection->traverse(FetchAllVisitor<ISprite>(sprites));
		selected = sprites[0];
	}
	if (!selected) return;

	Vector pos = m_editPanel->transPosScreenToProject(x, y);

	// offset
	if (m_is_offset_open)
	{
		d2d::Vector offset = selected->getPosition() + selected->getOffset();
		if (Math::getDistance(offset, pos) < SCALE_NODE_RADIUS) {
			delete m_op_state;
			m_op_state = CreateOffsetState(selected);
			return;
		}
	}

	// scale
	if (m_isDeformOpen)
	{
		Vector ctrlNodes[8];
		SpriteCtrlNode::GetSpriteCtrlNodes(selected, ctrlNodes);
		for (int i = 0; i < 8; ++i)
		{
			if (Math::getDistance(ctrlNodes[i], pos) < SCALE_NODE_RADIUS)
			{
				SpriteCtrlNode::Node cn;
				cn.pos = ctrlNodes[i];
				cn.type = SpriteCtrlNode::Type(i);
				delete m_op_state;
				m_op_state = CreateScaleState(selected, cn);
				return;
			}
		}
	}

	// translate
	m_op_state = CreateTransalteState(m_selection, pos);
}

void ArrangeSpriteImpl::onMouseLeftUp(int x, int y)
{
	Vector pos = m_editPanel->transPosScreenToProject(x, y);
	if (m_op_state) 
	{
		AbstractAtomicOP* history = m_op_state->OnMouseRelease(pos);
		m_editPanel->addHistoryOP(history);
	}

	if (Settings::bSpriteCapture && m_align.IsOpen() && !m_selection->empty())
	{
		std::vector<ISprite*> sprites;
		m_selection->traverse(FetchAllVisitor<ISprite>(sprites));
		m_align.Align(sprites);
	}

	if (m_propertyPanel)
	{
		m_propertyPanel->enablePropertyGrid(true);
		m_propertyPanel->updatePropertyGrid();
	}
}

void ArrangeSpriteImpl::onMouseRightDown(int x, int y)
{
	Vector pos = m_editPanel->transPosScreenToProject(x, y);
	m_right_down_pos = pos;

	ISprite* selected = NULL;
	if (m_selection->size() == 1)
	{
		std::vector<ISprite*> sprites;
		m_selection->traverse(FetchAllVisitor<ISprite>(sprites));
		selected = sprites[0];
	}
	if (!selected) return;

	// shear
	Vector ctrlNodes[8];
	SpriteCtrlNode::GetSpriteCtrlNodes(selected, ctrlNodes);
	for (int i = 0; i < 8; ++i)
	{
		if (Math::getDistance(ctrlNodes[i], pos) < SCALE_NODE_RADIUS)
		{
			SpriteCtrlNode::Node cn;
			cn.pos = ctrlNodes[i];
			cn.type = SpriteCtrlNode::Type(i);
			delete m_op_state;
			m_op_state = CreateShearState(selected, cn);
			return;
		}
	}

	// rotate
	m_op_state = CreateRotateState(m_selection, pos);
}

void ArrangeSpriteImpl::onMouseRightUp(int x, int y)
{
	if (m_right_down_pos.isValid() && !m_selection->empty())
	{
		Vector pos = m_editPanel->transPosScreenToProject(x, y);
		d2d::ISprite* sprite = m_spritesImpl->querySpriteByPos(pos);
		if (pos == m_right_down_pos && sprite)
		{
			wxMenu menu;
			setRightPopupMenu(menu);
			m_editPanel->PopupMenu(&menu, x, y);
		}
		else if (m_op_state)
		{
			AbstractAtomicOP* history = m_op_state->OnMouseRelease(pos);
			m_editPanel->addHistoryOP(history);
		}

		if (m_propertyPanel)
		{
			m_propertyPanel->enablePropertyGrid(true);
			m_propertyPanel->updatePropertyGrid();
		}
	}
}

void ArrangeSpriteImpl::onMouseDrag(int x, int y)
{
	Vector pos = m_editPanel->transPosScreenToProject(x, y);
	if (m_op_state && m_op_state->OnMousePress(pos))
	{
		if (m_propertyPanel) {
			m_propertyPanel->enablePropertyGrid(false);
		}
		m_editPanel->Refresh();
	}
}

void ArrangeSpriteImpl::onPopMenuSelected(int type)
{
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
}

void ArrangeSpriteImpl::onDraw() const
{
	if (m_isDeformOpen && m_selection->size() == 1)
	{
		ISprite* selected = NULL;
		std::vector<ISprite*> sprites;
		m_selection->traverse(FetchAllVisitor<ISprite>(sprites));
		selected = sprites[0];

		Vector ctrlNodes[8];
		SpriteCtrlNode::GetSpriteCtrlNodes(selected, ctrlNodes);
		for (int i = 0; i < 4; ++i)
			PrimitiveDraw::drawCircle(ctrlNodes[i], SCALE_NODE_RADIUS, false, 2, Colorf(0.2f, 0.8f, 0.2f));
		for (int i = 4; i < 8; ++i)
			PrimitiveDraw::drawCircle(ctrlNodes[i], SCALE_NODE_RADIUS, true, 2, Colorf(0.2f, 0.8f, 0.2f));

		if (m_is_offset_open)
		{
			d2d::Vector offset = selected->getPosition() + selected->getOffset();
			PrimitiveDraw::drawCircle(offset, SCALE_NODE_RADIUS, true, 2, Colorf(0.8f, 0.2f, 0.2f));
		}
	}

	m_align.Draw();
}

void ArrangeSpriteImpl::clear()
{
}

ISprite* ArrangeSpriteImpl::QueryEditedSprite(const Vector& pos) const
{
	ISprite* selected = NULL;
	if (m_isDeformOpen && m_selection->size() == 1)
	{
		std::vector<ISprite*> sprites;
		m_selection->traverse(FetchAllVisitor<ISprite>(sprites));
		selected = sprites[0];
	}
	if (!selected) return NULL;

	if (m_is_offset_open)
	{
		d2d::Vector offset = selected->getPosition() + selected->getOffset();
		if (Math::getDistance(offset, pos) < SCALE_NODE_RADIUS) {
			return selected;
		}
	}

	Vector ctrlNodes[8];
	SpriteCtrlNode::GetSpriteCtrlNodes(selected, ctrlNodes);
	for (int i = 0; i < 8; ++i) {
		if (Math::getDistance(ctrlNodes[i], pos) < SCALE_NODE_RADIUS) {
			return selected;
		}
	}
	return NULL;
}

void ArrangeSpriteImpl::onDirectionKeyDown(DirectionType type)
{
	if (!m_op_state) return;

	bool dirty = m_op_state->OnDirectionKeyDown(type);
	if (dirty)
	{
		if (m_propertyPanel) {
			m_propertyPanel->enablePropertyGrid(false);
		}
		m_editPanel->Refresh();
	}
}

void ArrangeSpriteImpl::onSpaceKeyDown()
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

void ArrangeSpriteImpl::setRightPopupMenu(wxMenu& menu)
{
	menu.Append(EditPanel::Menu_UpOneLayer, EditPanel::menu_entries[EditPanel::Menu_UpOneLayer]);
	menu.Append(EditPanel::Menu_DownOneLayer, EditPanel::menu_entries[EditPanel::Menu_DownOneLayer]);
}

IArrangeSpriteState* ArrangeSpriteImpl::CreateTransalteState(SpriteSelection* selection, const Vector& first_pos) const
{
	return new TranslateSpriteState(selection, first_pos);
}

IArrangeSpriteState* ArrangeSpriteImpl::CreateRotateState(SpriteSelection* selection, const Vector& first_pos) const
{
	return new RotateSpriteState(selection, first_pos);
}

IArrangeSpriteState* ArrangeSpriteImpl::CreateScaleState(ISprite* sprite, const SpriteCtrlNode::Node& ctrl_node) const
{
	return new ScaleSpriteState(sprite, ctrl_node);
}

IArrangeSpriteState* ArrangeSpriteImpl::CreateShearState(ISprite* sprite, const SpriteCtrlNode::Node& ctrl_node) const
{
	return new ShearSpriteState(sprite, ctrl_node);
}

IArrangeSpriteState* ArrangeSpriteImpl::CreateOffsetState(ISprite* sprite) const
{
	return new OffsetSpriteState(sprite);
}

}