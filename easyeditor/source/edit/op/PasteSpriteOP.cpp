#include "PasteSpriteOP.h"
#include "PasteSpriteCMPT.h"
#include "MultiSpritesImpl.h"
#include "StageCanvas.h"
#include "SpriteSelection.h"
#include "EditPanelImpl.h"
#include "SpriteRenderer.h"
#include "panel_msg.h"
#include "sprite_msg.h"
#include "FetchAllVisitor.h"

namespace ee
{

PasteSpriteOP::PasteSpriteOP(wxWindow* wnd, EditPanelImpl* stage, 
							 MultiSpritesImpl* sprites_impl,
							 PasteSpriteCMPT* cmpt)
	: SelectSpritesOP(wnd, stage, sprites_impl)
	, m_cmpt(cmpt)
{
	m_selection = sprites_impl->GetSpriteSelection();
	m_selection->AddReference();

	m_pos.MakeInvalid();
}

PasteSpriteOP::~PasteSpriteOP()
{
	m_selection->RemoveReference();
}

bool PasteSpriteOP::OnKeyDown(int keyCode)
{
	if (SelectSpritesOP::OnKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
		m_batch.Clear();

	return false;
}

bool PasteSpriteOP::OnMouseLeftDown(int x, int y)
{
	if (m_batch.Empty())
	{
		if (SelectSpritesOP::OnMouseLeftDown(x, y))
			return true;
	}
	else
	{
		if (ZoomViewOP::OnMouseLeftDown(x, y))
			return true;
	}

	setMousePos(x, y);

	if (m_cmpt)
		m_batch.InsertToSpritesImpl(m_pos, m_cmpt->isHorMirror(), m_cmpt->isVerMirror());
	else
		m_batch.InsertToSpritesImpl(m_pos, false, false);
	m_batch.LoadFromSelection(*m_selection);

	return false;
}

bool PasteSpriteOP::OnMouseLeftUp(int x, int y)
{
	if (m_batch.Empty())
	{
		if (SelectSpritesOP::OnMouseLeftUp(x, y))
			return true;
	}
	else
	{
		if (ZoomViewOP::OnMouseLeftUp(x, y))
			return true;
	}

	setMousePos(x, y);

	m_batch.LoadFromSelection(*m_selection);

	return false;
}

bool PasteSpriteOP::OnMouseRightDown(int x, int y)
{
	if (SelectSpritesOP::OnMouseRightDown(x, y)) return true;

	Clear();

	return false;
}

bool PasteSpriteOP::OnMouseMove(int x, int y)
{
	if (SelectSpritesOP::OnMouseMove(x, y)) return true;

	setMousePos(x, y);

	SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool PasteSpriteOP::OnDraw() const
{
	if (SelectSpritesOP::OnDraw()) return true;

	if (m_pos.IsValid()) {
		if (m_cmpt)
			m_batch.Draw(m_pos, m_cmpt->isHorMirror(), m_cmpt->isVerMirror());
		else
			m_batch.Draw(m_pos, false, false);
	}

	return false;
}

bool PasteSpriteOP::Clear()
{
	if (SelectSpritesOP::Clear()) return true;

	m_pos.MakeInvalid();

	m_batch.Clear();

	return false;
}

void PasteSpriteOP::setMousePos(int x, int y)
{
	m_pos = m_stage->TransPosScrToProj(x, y);
	if (m_stage->GetKeyState(WXK_SHIFT))
		fixPosOrthogonal();
}

void PasteSpriteOP::fixPosOrthogonal()
{
	if (m_pos.IsValid() && m_batch.IsCenterValid())
	{
		const sm::vec2& center = m_batch.GetCenter();
		if (fabs(m_pos.x - center.x) < fabs(m_pos.y - center.y))
			m_pos.x = center.x;
		else
			m_pos.y = center.y;
	}
}

//////////////////////////////////////////////////////////////////////////
// class PasteSpriteOP::SpriteBatch
//////////////////////////////////////////////////////////////////////////

PasteSpriteOP::SpriteBatch::
SpriteBatch()
	: m_valid(false)
{
}

PasteSpriteOP::SpriteBatch::
~SpriteBatch()
{
	Clear();
}

void PasteSpriteOP::SpriteBatch::
LoadFromSelection(const SpriteSelection& selection)
{
	if (!selection.IsEmpty())
	{	
		Clear();
		selection.Traverse(FetchAllVisitor<Sprite>(m_selected));
		ComputeCenter();
	}
}

void PasteSpriteOP::SpriteBatch::
InsertToSpritesImpl(const sm::vec2& pos, bool isHorMirror, bool isVerMirror)
{
	for (size_t i = 0, n = m_selected.size(); i < n; ++i)
	{
		Sprite* spr = m_selected[i];
		sm::vec2 fixed = spr->GetPosition() - m_center;
		if (isHorMirror)
			fixed.x += (m_center.x - spr->GetPosition().x) * 2;
		if (isVerMirror)
			fixed.y += (m_center.y - spr->GetPosition().y) * 2;

//		sprites_impl->insertSprite(spr->getSymbol(), fixed + pos);

		Sprite* newOne = dynamic_cast<Sprite*>(((cu::Cloneable*)spr)->Clone());
		newOne->SetPosition(fixed + pos);
		InsertSpriteSJ::Instance()->Insert(newOne);
		newOne->RemoveReference();
	}
}

void PasteSpriteOP::SpriteBatch::
Draw(const sm::vec2& pos, bool isHorMirror, bool isVerMirror) const
{
	if (!m_selected.empty() && m_valid)
	{
		const float xOffset = pos.x - m_center.x,
			yOffset = pos.y - m_center.y;

		for (size_t i = 0, n = m_selected.size(); i < n; ++i)
		{
			const sm::vec2& pos = m_selected[i]->GetPosition();

			float x = xOffset, y = yOffset;
			if (isHorMirror)
				x += (m_center.x - pos.x) * 2;
			if (isVerMirror)
				y += (m_center.y - pos.y) * 2;

			s2::RenderParams params;
			params.mt = sm::mat4::Translated(x, y, 0);
			SpriteRenderer::Instance()->Draw(m_selected[i], params);
		}
	}
}

void PasteSpriteOP::SpriteBatch::
Clear()
{
	m_selected.clear();
	m_valid = false;
}

void PasteSpriteOP::SpriteBatch::
ComputeCenter()
{
	m_center.Set(0, 0);
	for (size_t i = 0, n = m_selected.size(); i < n; ++i)
		m_center += m_selected[i]->GetPosition();
	m_center /= m_selected.size();
	m_valid = true;
}

}