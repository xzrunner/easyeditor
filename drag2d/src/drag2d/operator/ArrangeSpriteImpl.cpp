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
#include "view/Camera.h"
#include "history/DeleteSpriteAOP.h"
#include "history/CombineAOP.h"
#include "history/TranslateSpriteAOP.h"
#include "history/ScaleSpriteAOP.h"
#include "history/ShearSpriteAOP.h"
#include "history/OffsetSpriteAOP.h"
#include "render/PrimitiveDraw.h"
#include "render/DynamicTexAndFont.h"

// for debug
#include "render/ShaderMgr.h"

namespace d2d
{

const float ArrangeSpriteImpl::CTRL_NODE_RADIUS = 10.0f;

ArrangeSpriteImpl::ArrangeSpriteImpl(EditPanel* editPanel,
									 MultiSpritesImpl* spritesImpl,
									 PropertySettingPanel* propertyPanel,
									 const ArrangeSpriteConfig& cfg) 
	: m_stage(editPanel)
	, m_spritesImpl(spritesImpl)
	, m_propertyPanel(propertyPanel)
	, m_align(spritesImpl)
	, m_op_state(NULL)
	, m_cfg(cfg)
{
	m_align.SetOpen(cfg.is_auto_align_open);

	m_selection = spritesImpl->getSpriteSelection();
	m_selection->Retain();

	m_left_down_pos.setInvalid();
	m_right_down_pos.setInvalid();

	m_ctrl_node_radius = CTRL_NODE_RADIUS;
}

ArrangeSpriteImpl::~ArrangeSpriteImpl()
{
	m_selection->Release();
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
			m_selection->Traverse(FetchAllVisitor<ISprite>(sprites));
			m_stage->addHistoryOP(new DeleteSpriteAOP(sprites, m_spritesImpl));
		}
		m_spritesImpl->removeSpriteSelection();
		break;
	case WXK_PAGEUP:
		UpOneLayer();
		break;
	case WXK_PAGEDOWN:
		DownOneLayer();
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
		ShaderMgr::Instance()->SetBufferData(true);
		break;
	case 'C':
		ShaderMgr::Instance()->SetBufferData(false);
		break;
	}
}

void ArrangeSpriteImpl::onKeyUp(int keyCode)
{
	switch (keyCode)
	{
	case WXK_SPACE:
		{
			std::vector<ISprite*> sprites;
			m_selection->Traverse(FetchAllVisitor<ISprite>(sprites));
			CombineAOP* comb = new CombineAOP();
			for (int i = 0, n = sprites.size(); i < n; ++i) {
				ISprite* sprite = sprites[i];

				comb->Insert(new TranslateSpriteAOP(sprite, -sprite->getPosition()));
				comb->Insert(new ScaleSpriteAOP(sprite, Vector(1, 1), sprite->getScale()));
				comb->Insert(new ShearSpriteAOP(sprite, Vector(0, 0), sprite->getShear()));
				//				comb->Insert(new OffsetSpriteAOP(sprite, Vector(0, 0), sprite->getOffset()));

				sprite->setTransform(Vector(0, 0), 0);
				sprite->setScale(1, 1);
				sprite->setShear(0, 0);
				//				sprite->setOffset(Vector(0, 0));
			}
			m_stage->addHistoryOP(comb);
		}
		break;
	}

	if (m_propertyPanel)
	{
		m_propertyPanel->EnablePropertyGrid(true);
		m_propertyPanel->UpdatePropertyGrid();
	}
}

void ArrangeSpriteImpl::onMouseLeftDown(int x, int y)
{
	Vector pos = m_stage->transPosScreenToProject(x, y);
	m_left_down_pos = pos;

	m_align.SetInvisible();

	ISprite* selected = NULL;
	if (m_selection->Size() == 1)
	{
		std::vector<ISprite*> sprites;
		m_selection->Traverse(FetchAllVisitor<ISprite>(sprites));
		selected = sprites[0];
	}
	if (!selected) {
		if (m_op_state) {
			m_op_state->OnMousePress(pos);
		}
		return;
	}

	// offset
	if (m_cfg.is_offset_open)
	{
		d2d::Vector offset = selected->getPosition() + selected->getOffset();
		if (Math::getDistance(offset, pos) < m_ctrl_node_radius) {
			delete m_op_state;
			m_op_state = CreateOffsetState(selected);
			return;
		}
	}

	// scale
	if (m_cfg.is_deform_open)
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
	Vector pos = m_stage->transPosScreenToProject(x, y);
	if (m_op_state) 
	{
		AbstractAtomicOP* history = m_op_state->OnMouseRelease(pos);
		if (history) {
			m_stage->addHistoryOP(history);
		}

		delete m_op_state;
		m_op_state = NULL;
	}

	if (!m_selection->IsEmpty()) {
		Vector p;
		p.setInvalid();
		m_op_state = CreateTransalteState(m_selection, p);
	}

	if (Config::Instance()->GetSettings().open_sprite_capture
		&& m_align.IsOpen() 
		&& !m_selection->IsEmpty()
		&& m_left_down_pos != pos)
	{
		std::vector<ISprite*> sprites;
		m_selection->Traverse(FetchAllVisitor<ISprite>(sprites));
		m_align.Align(sprites);
	}

	if (m_propertyPanel)
	{
		m_propertyPanel->EnablePropertyGrid(true);
		m_propertyPanel->UpdatePropertyGrid();
	}
}

void ArrangeSpriteImpl::onMouseRightDown(int x, int y)
{
	Vector pos = m_stage->transPosScreenToProject(x, y);
	m_right_down_pos = pos;

	ISprite* selected = NULL;
	if (m_selection->Size() == 1)
	{
		std::vector<ISprite*> sprites;
		m_selection->Traverse(FetchAllVisitor<ISprite>(sprites));
		selected = sprites[0];
	}
	if (!selected) return;

	// shear
	if (m_cfg.is_deform_open)
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
				m_op_state = CreateShearState(selected, cn);
				return;
			}
		}
	}

	// rotate
	if (m_cfg.is_rotate_open) {
		m_op_state = CreateRotateState(m_selection, pos);
	}
}

void ArrangeSpriteImpl::onMouseRightUp(int x, int y)
{
	if (m_right_down_pos.isValid() && !m_selection->IsEmpty())
	{
		Vector pos = m_stage->transPosScreenToProject(x, y);
		d2d::ISprite* sprite = m_spritesImpl->querySpriteByPos(pos);
		if (pos == m_right_down_pos && sprite)
		{
			wxMenu menu;
			SetRightPopupMenu(menu, sprite);
			m_stage->PopupMenu(&menu, x, y);
		}
		else if (m_op_state)
		{
			AbstractAtomicOP* history = m_op_state->OnMouseRelease(pos);
			if (history) {
				m_stage->addHistoryOP(history);
			}

			delete m_op_state;
			m_op_state = NULL;
		}

		if (m_propertyPanel)
		{
			m_propertyPanel->EnablePropertyGrid(true);
			m_propertyPanel->UpdatePropertyGrid();
		}
	}
}

void ArrangeSpriteImpl::onMouseDrag(int x, int y)
{
	Vector pos = m_stage->transPosScreenToProject(x, y);
	if (m_op_state && m_op_state->OnMouseMove(pos))
	{
		if (m_propertyPanel) {
			m_propertyPanel->EnablePropertyGrid(false);
		}
		m_stage->Refresh();
	}
}

void ArrangeSpriteImpl::onPopMenuSelected(int type)
{
	switch (type)
	{
	case MENU_UP_ONE_LAYER:
		UpOneLayer();
		break;
	case MENU_DOWN_ONE_LAYER:
		DownOneLayer();
		break;
	case MENU_UP_MOST:
		UpLayerMost();
		break;
	case MENU_DOWN_MOST:
		DownLayerMost();
		break;
	case MENU_HORI_MIRROR:
		HoriMirror();
		break;
	case MENU_VERT_MIRROR:
		VertMirror();
		break;
	case MENU_INSERT_TO_DTEX:
		{
			if (Config::Instance()->IsUseDTex()) {
				std::vector<d2d::ISprite*> selected;
				m_selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(selected));

				DynamicTexAndFont* dtex = DynamicTexAndFont::Instance();
				for (size_t i = 0, n = selected.size(); i < n; ++i) {
					ISymbol& s = const_cast<ISymbol&>(selected[i]->getSymbol());
					dtex->InsertSymbol(s);
				}

				m_stage->getCanvas()->resetViewport();
			}
		}
		break;
	case MENU_REMOVE_FROM_DTEX:
		{
			if (Config::Instance()->IsUseDTex()) {
				std::vector<d2d::ISprite*> selected;
				m_selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(selected));
				//DynamicTexture* dtex = DynamicTexture::Instance();
				DynamicTexAndFont* dtex = DynamicTexAndFont::Instance();
				for (size_t i = 0, n = selected.size(); i < n; ++i) {
					ISymbol& s = const_cast<ISymbol&>(selected[i]->getSymbol());
					dtex->Remove(s.getFilepath());
				}
			}
		}
		break;
	}
}

void ArrangeSpriteImpl::onDraw(const Camera& cam) const
{
	m_ctrl_node_radius = CTRL_NODE_RADIUS * cam.GetScale();

	if (m_cfg.is_deform_open && m_selection->Size() == 1)
	{
		ISprite* selected = NULL;
		std::vector<ISprite*> sprites;
		m_selection->Traverse(FetchAllVisitor<ISprite>(sprites));
		selected = sprites[0];

		Vector ctrlNodes[8];
		SpriteCtrlNode::GetSpriteCtrlNodes(selected, ctrlNodes);
		for (int i = 0; i < 4; ++i)
			PrimitiveDraw::drawCircle(ctrlNodes[i], m_ctrl_node_radius, false, 2, Colorf(0.2f, 0.8f, 0.2f));
		for (int i = 4; i < 8; ++i)
			PrimitiveDraw::drawCircle(ctrlNodes[i], m_ctrl_node_radius, true, 2, Colorf(0.2f, 0.8f, 0.2f));

		if (m_cfg.is_offset_open)
		{
			d2d::Vector offset = selected->getPosition() + selected->getOffset();
			PrimitiveDraw::drawCircle(offset, m_ctrl_node_radius, true, 2, Colorf(0.8f, 0.2f, 0.2f));
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
	if (m_cfg.is_deform_open && m_selection->Size() == 1)
	{
		std::vector<ISprite*> sprites;
		m_selection->Traverse(FetchAllVisitor<ISprite>(sprites));
		selected = sprites[0];
	}
	if (!selected) return NULL;

	if (m_cfg.is_offset_open)
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

bool ArrangeSpriteImpl::IsSelectionEmpty() const
{
	return m_selection->IsEmpty();
}

void ArrangeSpriteImpl::onDirectionKeyDown(DirectionType type)
{
	if (!m_op_state) return;

	bool dirty = m_op_state->OnDirectionKeyDown(type);
	if (dirty)
	{
		if (m_propertyPanel) {
			m_propertyPanel->EnablePropertyGrid(false);
		}
		m_stage->Refresh();
	}
}

void ArrangeSpriteImpl::onSpaceKeyDown()
{
	std::vector<ISprite*> selected;
	m_selection->Traverse(FetchAllVisitor<ISprite>(selected));
	for (size_t i = 0, n = selected.size(); i < n; ++i)
	{
		selected[i]->setTransform(Vector(0, 0), 0);
		//selected[i]->setOffset(Vector(0, 0));
	}
	m_stage->Refresh();
}

void ArrangeSpriteImpl::SetRightPopupMenu(wxMenu& menu, ISprite* spr)
{
	m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED, &d2d::EditPanel::OnRightPopupMenu, m_stage, MENU_UP_ONE_LAYER);
	menu.Append(MENU_UP_ONE_LAYER, "上移一层");
	m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED, &d2d::EditPanel::OnRightPopupMenu, m_stage, MENU_DOWN_ONE_LAYER);
	menu.Append(MENU_DOWN_ONE_LAYER, "下移一层");

	menu.AppendSeparator();

	m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED, &d2d::EditPanel::OnRightPopupMenu, m_stage, MENU_UP_MOST);
	menu.Append(MENU_UP_MOST, "移到顶");
	m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED, &d2d::EditPanel::OnRightPopupMenu, m_stage, MENU_DOWN_MOST);
	menu.Append(MENU_DOWN_MOST, "移到底");

	menu.AppendSeparator();

	m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED, &d2d::EditPanel::OnRightPopupMenu, m_stage, MENU_HORI_MIRROR);
	menu.Append(MENU_HORI_MIRROR, "水平镜像");
	m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED, &d2d::EditPanel::OnRightPopupMenu, m_stage, MENU_VERT_MIRROR);
	menu.Append(MENU_VERT_MIRROR, "竖直镜像");	

#ifdef _DEBUG
	menu.AppendSeparator();
	m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED, &d2d::EditPanel::OnRightPopupMenu, m_stage, MENU_INSERT_TO_DTEX);
	menu.Append(MENU_INSERT_TO_DTEX, "Insert To DTex");
	m_stage->Bind(wxEVT_COMMAND_MENU_SELECTED, &d2d::EditPanel::OnRightPopupMenu, m_stage, MENU_REMOVE_FROM_DTEX);
	menu.Append(MENU_REMOVE_FROM_DTEX, "Remove From DTex");
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
	m_selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(selected));
	for (size_t i = 0, n = selected.size(); i < n; ++i) {
		m_spritesImpl->resetSpriteOrder(selected[i], true);
	}
}

void ArrangeSpriteImpl::DownOneLayer()
{
	std::vector<d2d::ISprite*> selected;
	m_selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(selected));
	for (size_t i = 0, n = selected.size(); i < n; ++i) {
		m_spritesImpl->resetSpriteOrder(selected[i], false);
	}
}

void ArrangeSpriteImpl::UpLayerMost()
{
	std::vector<d2d::ISprite*> selected;
	m_selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(selected));
	for (size_t i = 0, n = selected.size(); i < n; ++i) {
		do {
		} while (m_spritesImpl->resetSpriteOrder(selected[i], true));
	}
}

void ArrangeSpriteImpl::DownLayerMost()
{
	std::vector<d2d::ISprite*> selected;
	m_selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(selected));
	for (size_t i = 0, n = selected.size(); i < n; ++i) {
		do {
		} while (m_spritesImpl->resetSpriteOrder(selected[i], false));
	}
}

void ArrangeSpriteImpl::HoriMirror()
{
	std::vector<d2d::ISprite*> selected;
	m_selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(selected));
	for (size_t i = 0, n = selected.size(); i < n; ++i) {
		d2d::ISprite* spr = selected[i];
		spr->setMirror(!spr->getMirrorX(), spr->getMirrorY());
	}
}

void ArrangeSpriteImpl::VertMirror()
{
	std::vector<d2d::ISprite*> selected;
	m_selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(selected));
	for (size_t i = 0, n = selected.size(); i < n; ++i) {
		d2d::ISprite* spr = selected[i];
		spr->setMirror(spr->getMirrorX(), !spr->getMirrorY());
	}
}

}