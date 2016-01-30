#include "PasteSpriteOP.h"

#include "common/visitors.h"
#include "component/PasteSpriteCMPT.h"
#include "render/GL10.h"
#include "view/MultiSpritesImpl.h"
#include "view/IStageCanvas.h"
#include "view/SpriteSelection.h"
#include "view/EditPanelImpl.h"
#include "render/SpriteRenderer.h"
#include "message/InsertSpriteSJ.h"
#include "message/SetCanvasDirtySJ.h"

namespace d2d
{

PasteSpriteOP::PasteSpriteOP(wxWindow* wnd, EditPanelImpl* stage, 
							 MultiSpritesImpl* spritesImpl,
							 PasteSpriteCMPT* cmpt)
	: SelectSpritesOP(wnd, stage, spritesImpl)
	, m_cmpt(cmpt)
{
	m_selection = spritesImpl->GetSpriteSelection();
	m_selection->Retain();

	m_pos.SetInvalid();
}

PasteSpriteOP::~PasteSpriteOP()
{
	m_selection->Release();
}

bool PasteSpriteOP::OnKeyDown(int keyCode)
{
	if (SelectSpritesOP::OnKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE)
		m_batch.clear();

	return false;
}

bool PasteSpriteOP::OnMouseLeftDown(int x, int y)
{
	if (m_batch.empty())
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
		m_batch.insertToSpritesImpl(m_pos, m_cmpt->isHorMirror(), m_cmpt->isVerMirror());
	else
		m_batch.insertToSpritesImpl(m_pos, false, false);
	m_batch.loadFromSelection(*m_selection);

	return false;
}

bool PasteSpriteOP::OnMouseLeftUp(int x, int y)
{
	if (m_batch.empty())
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

	m_batch.loadFromSelection(*m_selection);

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

	if (m_cmpt)
		m_batch.draw(m_pos, m_cmpt->isHorMirror(), m_cmpt->isVerMirror());
	else
		m_batch.draw(m_pos, false, false);

	return false;
}

bool PasteSpriteOP::Clear()
{
	if (SelectSpritesOP::Clear()) return true;

	m_pos.SetInvalid();
	m_batch.clear();

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
	const Vector& center = m_batch.getCenter();
	if (m_pos.IsValid() && center.IsValid())
	{
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
{
	m_center.SetInvalid();
}

PasteSpriteOP::SpriteBatch::
~SpriteBatch()
{
	clear();
}

void PasteSpriteOP::SpriteBatch::
loadFromSelection(const SpriteSelection& selection)
{
	if (!selection.IsEmpty())
	{	
		clear();
		selection.Traverse(FetchAllVisitor<Sprite>(m_selected));
		computeCenter();
	}
}

void PasteSpriteOP::SpriteBatch::
insertToSpritesImpl(const Vector& pos, bool isHorMirror, bool isVerMirror)
{
	for (size_t i = 0, n = m_selected.size(); i < n; ++i)
	{
		Sprite* sprite = m_selected[i];
		Vector fixed = sprite->GetPosition() - m_center;
		if (isHorMirror)
			fixed.x += (m_center.x - sprite->GetPosition().x) * 2;
		if (isVerMirror)
			fixed.y += (m_center.y - sprite->GetPosition().y) * 2;

//		spritesImpl->insertSprite(sprite->getSymbol(), fixed + pos);

		Sprite* newOne = sprite->Clone();
		newOne->SetTransform(fixed + pos, newOne->GetAngle());
		InsertSpriteSJ::Instance()->Insert(newOne);
		newOne->Release();
	}
}

void PasteSpriteOP::SpriteBatch::
draw(const Vector& pos, bool isHorMirror, bool isVerMirror) const
{
	if (!m_selected.empty() && pos.IsValid())
	{
		const float xOffset = pos.x - m_center.x,
			yOffset = pos.y - m_center.y;

		for (size_t i = 0, n = m_selected.size(); i < n; ++i)
		{
			const Vector& pos = m_selected[i]->GetPosition();

			float x = xOffset, y = yOffset;
			if (isHorMirror)
				x += (m_center.x - pos.x) * 2;
			if (isVerMirror)
				y += (m_center.y - pos.y) * 2;

			GL10::PushMatrix();
			GL10::Translatef(x, y, 0.0f);
			SpriteRenderer::Instance()->Draw(m_selected[i]);
			GL10::PopMatrix();
		}
	}
}

void PasteSpriteOP::SpriteBatch::
clear()
{
	m_selected.clear();
	m_center.SetInvalid();
}

void PasteSpriteOP::SpriteBatch::
computeCenter()
{
	m_center.Set(0, 0);
	for (size_t i = 0, n = m_selected.size(); i < n; ++i)
		m_center += m_selected[i]->GetPosition();
	m_center /= m_selected.size();
}

}