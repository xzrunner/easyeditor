#include "ArrangeSpriteImpl.h"

#include "TranslateSpriteState.h"
#include "RotateSpriteState.h"
#include "ScaleSpriteState.h"
#include "ShearSpriteState.h"
#include "OffsetSpriteState.h"
#include "MoveSpriteState.h"
#include "CopyPasteSpriteState.h"
#include "PerspectiveSpriteState.h"

#include "common/Math.h"
#include "common/Config.h"
#include "common/SettingData.h"
#include "dataset/AbstractBV.h"
#include "dataset/ISymbol.h"
#include "view/EditPanel.h"
#include "view/MultiSpritesImpl.h"
#include "view/PropertySettingPanel.h"
#include "view/Camera.h"
#include "view/IStageCanvas.h"
#include "view/SpriteSelection.h"
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
const float ArrangeSpriteImpl::MAX_CTRL_NODE_RADIUS = 10.0f;

ArrangeSpriteImpl::ArrangeSpriteImpl(EditPanel* editPanel,
									 MultiSpritesImpl* spritesImpl,
									 PropertySettingPanel* propertyPanel,
									 const ArrangeSpriteConfig& cfg) 
	: m_stage(editPanel)
	, m_sprites_impl(spritesImpl)
	, m_property_panel(propertyPanel)
	, m_align(spritesImpl)
	, m_op_state(NULL)
	, m_cfg(cfg)
{
	m_align.SetOpen(cfg.is_auto_align_open);

	m_selection = spritesImpl->GetSpriteSelection();
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

void ArrangeSpriteImpl::OnKeyDown(int keyCode)
{
	switch (keyCode)
	{
	case WXK_DELETE:
		OnDeleteKeyDown();
		break;
	case WXK_PAGEUP:
		UpOneLayer();
		break;
	case WXK_PAGEDOWN:
		DownOneLayer();
		break;
	case 'a': case 'A':
		OnDirectionKeyDown(e_left);
		break;
	case 'd': case 'D':
		OnDirectionKeyDown(e_right);
		break;
	case 's': case 'S':
		OnDirectionKeyDown(e_down);
		break;
	case 'w': case 'W':
		OnDirectionKeyDown(e_up);
		break;
	case '[':
		DownOneLayer();
		break;
	case ']':
		UpOneLayer();
		break;
	case 'm' : case 'M':
		m_op_state = new MoveSpriteState(m_selection);
		break;
	case WXK_SPACE:
		OnSpaceKeyDown();
		break;

// 		// for debug
// 	case 'O':
// 		ShaderMgr::Instance()->SetBufferData(true);
// 		break;
// 	case 'C':
// 		ShaderMgr::Instance()->SetBufferData(false);
// 		break;
	}
}

void ArrangeSpriteImpl::OnKeyUp(int keyCode)
{
	if (m_property_panel)
	{
		m_property_panel->EnablePropertyGrid(true);
		m_property_panel->UpdatePropertyGrid();
	}
}

void ArrangeSpriteImpl::OnMouseLeftDown(int x, int y)
{
	Vector pos = m_stage->TransPosScrToProj(x, y);
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

	// copy & paste
	if (m_stage->GetKeyState(WXK_ALT))
	{
		m_op_state = new CopyPasteSpriteState(m_sprites_impl, m_selection, selected);
	}

	// offset
	if (m_cfg.is_offset_open)
	{
		d2d::Vector offset = GetSprOffset(selected);
		if (Math::getDistance(offset, pos) < m_ctrl_node_radius) {
			delete m_op_state;
			m_op_state = CreateOffsetState(selected);
			return;
		}
	}

	// scale
	if (m_cfg.is_deform_open && !m_stage->GetKeyState(WXK_SHIFT))
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

	// perspective
	if (m_cfg.is_deform_open && m_stage->GetKeyState(WXK_SHIFT))
	{
		Vector ctrl_node[4];
		SpriteCtrlNode::GetSpriteCtrlNodesExt(selected, ctrl_node);
		for (int i = 0; i < 4; ++i) {
			if (Math::getDistance(ctrl_node[i], pos) < m_ctrl_node_radius) {
				SpriteCtrlNode::Node cn;
				cn.pos = ctrl_node[i];
				cn.type = SpriteCtrlNode::Type(i);
				delete m_op_state;
				m_op_state = CreatePerspectiveState(selected, cn);
				return;
			}
		}
	}

	// translate
	m_op_state = CreateTransalteState(m_selection, pos);

	m_op_state->OnMousePress(pos);
}

void ArrangeSpriteImpl::OnMouseLeftUp(int x, int y)
{
	Vector pos = m_stage->TransPosScrToProj(x, y);
	if (m_op_state) 
	{
		AbstractAtomicOP* history = m_op_state->OnMouseRelease(pos);
		if (history) {
			m_stage->AddOpRecord(history);
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

	if (m_property_panel)
	{
		m_property_panel->EnablePropertyGrid(true);
		m_property_panel->UpdatePropertyGrid();
	}
}

void ArrangeSpriteImpl::OnMouseRightDown(int x, int y)
{
	Vector pos = m_stage->TransPosScrToProj(x, y);
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

void ArrangeSpriteImpl::OnMouseRightUp(int x, int y)
{
	if (m_right_down_pos.isValid() && !m_selection->IsEmpty())
	{
		Vector pos = m_stage->TransPosScrToProj(x, y);
		d2d::ISprite* sprite = m_sprites_impl->QuerySpriteByPos(pos);
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
				m_stage->AddOpRecord(history);
			}

			delete m_op_state;
			m_op_state = NULL;
		}

		if (m_property_panel)
		{
			m_property_panel->EnablePropertyGrid(true);
			m_property_panel->UpdatePropertyGrid();
		}
	}
}

void ArrangeSpriteImpl::OnMouseMove(int x, int y)
{
	Vector pos = m_stage->TransPosScrToProj(x, y);
	if (m_op_state && m_op_state->OnMouseMove(pos))
	{
		if (m_property_panel) {
			m_property_panel->EnablePropertyGrid(false);
		}
	}
}

void ArrangeSpriteImpl::OnMouseDrag(int x, int y)
{
	Vector pos = m_stage->TransPosScrToProj(x, y);
	if (m_op_state && m_op_state->OnMouseDrag(pos))
	{
		m_stage->SetCanvasDirty();
		if (m_property_panel) {
			m_property_panel->EnablePropertyGrid(false);
		}
	}
}

void ArrangeSpriteImpl::OnPopMenuSelected(int type)
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
					ISymbol& s = const_cast<ISymbol&>(selected[i]->GetSymbol());
					dtex->InsertSymbol(s);
				}

				m_stage->GetCanvas()->ResetViewport();
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
					ISymbol& s = const_cast<ISymbol&>(selected[i]->GetSymbol());
					dtex->Remove(s.GetFilepath());
				}
			}
		}
		break;
	}
}

void ArrangeSpriteImpl::OnDraw(const Camera& cam) const
{
	m_ctrl_node_radius = std::min(CTRL_NODE_RADIUS * cam.GetScale(), MAX_CTRL_NODE_RADIUS);
	if (m_ctrl_node_radius / cam.GetScale() < 5) {
		m_ctrl_node_radius = 0;
	}

	if (m_cfg.is_deform_open && m_selection->Size() == 1)
	{
		ISprite* selected = NULL;
		std::vector<ISprite*> sprites;
		m_selection->Traverse(FetchAllVisitor<ISprite>(sprites));
		selected = sprites[0];

		Rect r = selected->GetRect();
		float min_e = std::min(r.xLength(), r.yLength());
		if (m_ctrl_node_radius > min_e * 0.1f) {
			m_ctrl_node_radius = 0;
			return;
		}

		if (m_stage->GetKeyState(WXK_SHIFT)) 
		{
			Vector ctrl_nodes[4];
			SpriteCtrlNode::GetSpriteCtrlNodesExt(selected, ctrl_nodes);
			for (int i = 0; i < 4; ++i)
				PrimitiveDraw::drawCircle(ctrl_nodes[i], m_ctrl_node_radius, true, 2, Colorf(0.2f, 0.8f, 0.2f));
		}
		else
		{
			Vector ctrl_nodes[8];
			SpriteCtrlNode::GetSpriteCtrlNodes(selected, ctrl_nodes);
			for (int i = 0; i < 4; ++i)
				PrimitiveDraw::drawCircle(ctrl_nodes[i], m_ctrl_node_radius, false, 2, Colorf(0.2f, 0.8f, 0.2f));
			for (int i = 4; i < 8; ++i)
				PrimitiveDraw::drawCircle(ctrl_nodes[i], m_ctrl_node_radius, true, 2, Colorf(0.2f, 0.8f, 0.2f));
		}

		if (m_cfg.is_offset_open)
		{
			d2d::Vector offset = GetSprOffset(selected);
			PrimitiveDraw::drawCircle(offset, m_ctrl_node_radius, true, 2, Colorf(0.8f, 0.2f, 0.2f));
		}
	}

	m_align.Draw();
}

void ArrangeSpriteImpl::Clear()
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
		d2d::Vector offset = GetSprOffset(selected);
		if (Math::getDistance(offset, pos) < m_ctrl_node_radius) {
			return selected;
		}
	}

	if (m_cfg.is_deform_open && !m_stage->GetKeyState(WXK_SHIFT))
	{
		Vector ctrl_nodes[8];
		SpriteCtrlNode::GetSpriteCtrlNodes(selected, ctrl_nodes);
		for (int i = 0; i < 8; ++i) {
			if (Math::getDistance(ctrl_nodes[i], pos) < m_ctrl_node_radius) {
				return selected;
			}
		}
	}

	if (m_cfg.is_deform_open && m_stage->GetKeyState(WXK_SHIFT))
	{
		Vector ctrl_nodes[4];
		SpriteCtrlNode::GetSpriteCtrlNodesExt(selected, ctrl_nodes);
		for (int i = 0; i < 4; ++i) {
			if (Math::getDistance(ctrl_nodes[i], pos) < m_ctrl_node_radius) {
				return selected;
			}
		}
	}

	return NULL;
}

bool ArrangeSpriteImpl::IsSelectionEmpty() const
{
	return m_selection->IsEmpty();
}

void ArrangeSpriteImpl::OnDirectionKeyDown(DirectionType type)
{
	if (!m_op_state) return;

	bool dirty = m_op_state->OnDirectionKeyDown(type);
	if (dirty)
	{
		if (m_property_panel) {
			m_property_panel->EnablePropertyGrid(false);
		}
	}
}

void ArrangeSpriteImpl::OnSpaceKeyDown()
{
	std::vector<ISprite*> sprites;
	m_selection->Traverse(FetchAllVisitor<ISprite>(sprites));
	CombineAOP* comb = new CombineAOP();
	for (int i = 0, n = sprites.size(); i < n; ++i) 
	{
		ISprite* sprite = sprites[i];

		comb->Insert(new TranslateSpriteAOP(sprite, -sprite->GetPosition()));
		comb->Insert(new ScaleSpriteAOP(sprite, Vector(1, 1), sprite->GetScale()));
		comb->Insert(new ShearSpriteAOP(sprite, Vector(0, 0), sprite->GetShear()));
		//comb->Insert(new OffsetSpriteAOP(sprite, Vector(0, 0), sprite->getOffset()));

		sprite->SetTransform(Vector(0, 0), 0);
		sprite->SetScale(1, 1);
		sprite->SetShear(0, 0);
		//sprite->setOffset(Vector(0, 0));
	}
	m_stage->AddOpRecord(comb);
	m_stage->SetCanvasDirty();
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

IArrangeSpriteState* ArrangeSpriteImpl::CreatePerspectiveState(ISprite* sprite, const SpriteCtrlNode::Node& ctrl_node) const
{
	return new PerspectiveSpriteState(sprite, ctrl_node);
}

void ArrangeSpriteImpl::OnDeleteKeyDown()
{
	// add to history
	std::vector<ISprite*> sprites;
	m_selection->Traverse(FetchAllVisitor<ISprite>(sprites));
	m_stage->AddOpRecord(new DeleteSpriteAOP(sprites, m_sprites_impl));

	m_sprites_impl->ClearSpriteSelection();
}

void ArrangeSpriteImpl::UpOneLayer()
{
	std::vector<d2d::ISprite*> selected;
	m_selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(selected));
	for (size_t i = 0, n = selected.size(); i < n; ++i) {
		m_sprites_impl->ReorderSprite(selected[i], true);
	}
}

void ArrangeSpriteImpl::DownOneLayer()
{
	std::vector<d2d::ISprite*> selected;
	m_selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(selected));
	for (size_t i = 0, n = selected.size(); i < n; ++i) {
		m_sprites_impl->ReorderSprite(selected[i], false);
	}
}

void ArrangeSpriteImpl::UpLayerMost()
{
	std::vector<d2d::ISprite*> selected;
	m_selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(selected));
	for (size_t i = 0, n = selected.size(); i < n; ++i) {
		do {
		} while (m_sprites_impl->ReorderSprite(selected[i], true));
	}
}

void ArrangeSpriteImpl::DownLayerMost()
{
	std::vector<d2d::ISprite*> selected;
	m_selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(selected));
	for (size_t i = 0, n = selected.size(); i < n; ++i) {
		do {
		} while (m_sprites_impl->ReorderSprite(selected[i], false));
	}
}

void ArrangeSpriteImpl::HoriMirror()
{
	std::vector<d2d::ISprite*> selected;
	m_selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(selected));
	for (size_t i = 0, n = selected.size(); i < n; ++i) {
		d2d::ISprite* spr = selected[i];
		spr->SetMirror(!spr->GetMirrorX(), spr->GetMirrorY());
	}
}

void ArrangeSpriteImpl::VertMirror()
{
	std::vector<d2d::ISprite*> selected;
	m_selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(selected));
	for (size_t i = 0, n = selected.size(); i < n; ++i) {
		d2d::ISprite* spr = selected[i];
		spr->SetMirror(spr->GetMirrorX(), !spr->GetMirrorY());
	}
}

d2d::Vector ArrangeSpriteImpl::GetSprOffset(const ISprite* spr) const
{
	d2d::Vector offset = spr->GetPosition() + spr->GetOffset();
	return offset;
}

}