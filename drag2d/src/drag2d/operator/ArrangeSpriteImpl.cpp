#include "ArrangeSpriteImpl.h"

#include "TranslateSpriteState.h"
#include "RotateSpriteState.h"
#include "ScaleSpriteState.h"
#include "ShearSpriteState.h"
#include "OffsetSpriteState.h"

#include "common/Math.h"
#include "common/Config.h"
#include "common/SettingData.h"
#include "dataset/ISymbol.h"
#include "view/EditPanel.h"
#include "view/MultiSpritesImpl.h"
#include "view/PropertySettingPanel.h"
#include "view/GLCanvas.h"
#include "history/DeleteSpriteAOP.h"
#include "render/PrimitiveDraw.h"
#include "render/DynamicTexAndFont.h"

// for debug
#include "render/ShaderNew.h"

namespace d2d
{

const float ArrangeSpriteImpl::CTRL_NODE_RADIUS = 10.0f;

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

	m_left_down_pos.setInvalid();
	m_right_down_pos.setInvalid();

	m_ctrl_node_radius = CTRL_NODE_RADIUS;
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
	case WXK_SPACE:
		{
			std::vector<ISprite*> sprites;
			m_selection->traverse(FetchAllVisitor<ISprite>(sprites));
			for (int i = 0, n = sprites.size(); i < n; ++i) {
				ISprite* sprite = sprites[i];
				sprite->setTransform(Vector(0, 0), 0);
				sprite->setScale(1, 1);
				sprite->setShear(0, 0);
//				sprite->setOffset(Vector(0, 0));
			}
		}
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
	case '[':
		DownOneLayer();
		break;
	case ']':
		UpOneLayer();
		break;

		// for debug
	case 'O':
		ShaderNew::Instance()->SetBufferData(true);
		break;
	case 'C':
		ShaderNew::Instance()->SetBufferData(false);
		break;
	}
}

void ArrangeSpriteImpl::onKeyUp(int keyCode)
{
	switch (keyCode)
	{
	case WXK_PAGEUP:
		UpOneLayer();
		break;
	case WXK_PAGEDOWN:
		DownOneLayer();
		break;
	}

	if (m_propertyPanel)
	{
		m_propertyPanel->enablePropertyGrid(true);
		m_propertyPanel->updatePropertyGrid();
	}
}

void ArrangeSpriteImpl::onMouseLeftDown(int x, int y)
{
	Vector pos = m_editPanel->transPosScreenToProject(x, y);
	m_left_down_pos = pos;

	m_align.SetInvisible();

	ISprite* selected = NULL;
	if (m_selection->size() == 1)
	{
		std::vector<ISprite*> sprites;
		m_selection->traverse(FetchAllVisitor<ISprite>(sprites));
		selected = sprites[0];
	}
	if (!selected) {
		if (m_op_state) {
			m_op_state->OnMousePress(pos);
		}
		return;
	}

	// offset
	if (m_is_offset_open)
	{
		d2d::Vector offset = selected->getPosition() + selected->getOffset();
		if (Math::getDistance(offset, pos) < m_ctrl_node_radius) {
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
			if (Math::getDistance(ctrlNodes[i], pos) < m_ctrl_node_radius)
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
	m_op_state->OnMousePress(pos);
}

void ArrangeSpriteImpl::onMouseLeftUp(int x, int y)
{
	Vector pos = m_editPanel->transPosScreenToProject(x, y);
	if (m_op_state) 
	{
		AbstractAtomicOP* history = m_op_state->OnMouseRelease(pos);
		if (history) {
			m_editPanel->addHistoryOP(history);
		}

		delete m_op_state;
		m_op_state = NULL;
	}

	if (!m_selection->empty()) {
		m_op_state = CreateTransalteState(m_selection, pos);
	}

	if (Config::Instance()->GetSettings().open_sprite_capture
		&& m_align.IsOpen() 
		&& !m_selection->empty()
		&& m_left_down_pos != pos)
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
		if (Math::getDistance(ctrlNodes[i], pos) < m_ctrl_node_radius)
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
			if (history) {
				m_editPanel->addHistoryOP(history);
			}

			delete m_op_state;
			m_op_state = NULL;
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
	if (m_op_state && m_op_state->OnMouseMove(pos))
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
		UpOneLayer();
		break;
	case EditPanel::Menu_DownOneLayer:
		DownOneLayer();
		break;
	case EditPanel::Menu_InsertToDTex:
		{
			std::vector<d2d::ISprite*> selected;
			m_selection->traverse(d2d::FetchAllVisitor<d2d::ISprite>(selected));
			DynamicTexAndFont* dtex = DynamicTexAndFont::Instance();
			for (size_t i = 0, n = selected.size(); i < n; ++i) {
				ISymbol& s = const_cast<ISymbol&>(selected[i]->getSymbol());
				dtex->InsertSymbol(s);
			}
			m_editPanel->getCanvas()->resetViewport();
		}
		break;
	case EditPanel::Menu_RemoveFromDTex:
		{
			std::vector<d2d::ISprite*> selected;
			m_selection->traverse(d2d::FetchAllVisitor<d2d::ISprite>(selected));
			//DynamicTexture* dtex = DynamicTexture::Instance();
			DynamicTexAndFont* dtex = DynamicTexAndFont::Instance();
			for (size_t i = 0, n = selected.size(); i < n; ++i) {
				ISymbol& s = const_cast<ISymbol&>(selected[i]->getSymbol());
				dtex->Remove(s.getFilepath());
			}
		}
		break;
	}
}

void ArrangeSpriteImpl::onDraw(const Screen& scr) const
{
	Vector scale = scr.GetScale();
	m_ctrl_node_radius = CTRL_NODE_RADIUS / scale.x;

	if (m_isDeformOpen && m_selection->size() == 1)
	{
		ISprite* selected = NULL;
		std::vector<ISprite*> sprites;
		m_selection->traverse(FetchAllVisitor<ISprite>(sprites));
		selected = sprites[0];

		Vector ctrlNodes[8];
		SpriteCtrlNode::GetSpriteCtrlNodes(selected, ctrlNodes);
		for (int i = 0; i < 4; ++i)
			PrimitiveDraw::drawCircle(ctrlNodes[i], m_ctrl_node_radius, false, 2, Colorf(0.2f, 0.8f, 0.2f));
		for (int i = 4; i < 8; ++i)
			PrimitiveDraw::drawCircle(ctrlNodes[i], m_ctrl_node_radius, true, 2, Colorf(0.2f, 0.8f, 0.2f));

		if (m_is_offset_open)
		{
			d2d::Vector offset = selected->getPosition() + selected->getOffset();
			PrimitiveDraw::drawCircle(offset, m_ctrl_node_radius, true, 2, Colorf(0.8f, 0.2f, 0.2f));
		}
	}

	m_align.Draw(scr);
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
		if (Math::getDistance(offset, pos) < m_ctrl_node_radius) {
			return selected;
		}
	}

	Vector ctrlNodes[8];
	SpriteCtrlNode::GetSpriteCtrlNodes(selected, ctrlNodes);
	for (int i = 0; i < 8; ++i) {
		if (Math::getDistance(ctrlNodes[i], pos) < m_ctrl_node_radius) {
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

#ifdef _DEBUG
	menu.Append(EditPanel::Menu_InsertToDTex, EditPanel::menu_entries[EditPanel::Menu_InsertToDTex]);
	menu.Append(EditPanel::Menu_RemoveFromDTex, EditPanel::menu_entries[EditPanel::Menu_RemoveFromDTex]);
#endif
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

void ArrangeSpriteImpl::UpOneLayer()
{
	std::vector<d2d::ISprite*> selected;
	m_selection->traverse(d2d::FetchAllVisitor<d2d::ISprite>(selected));
	for (size_t i = 0, n = selected.size(); i < n; ++i)
		m_spritesImpl->resetSpriteOrder(selected[i], true);
}

void ArrangeSpriteImpl::DownOneLayer()
{
	std::vector<d2d::ISprite*> selected;
	m_selection->traverse(d2d::FetchAllVisitor<d2d::ISprite>(selected));
	for (size_t i = 0, n = selected.size(); i < n; ++i)
		m_spritesImpl->resetSpriteOrder(selected[i], false);
}

}