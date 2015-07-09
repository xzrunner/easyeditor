#include "PasteSpriteOP.h"

#include "common/visitors.h"
#include "component/PasteSpriteCMPT.h"
#include "render/GL10.h"
#include "view/MultiSpritesImpl.h"
#include "view/IStageCanvas.h"
#include "render/SpriteRenderer.h"

namespace d2d
{

PasteSpriteOP::PasteSpriteOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl,
							 ViewPanelMgr* view_panel_mgr, PasteSpriteCMPT* cmpt)
	: SelectSpritesOP(editPanel, spritesImpl, view_panel_mgr)
	, m_spritesImpl(spritesImpl)
	, m_cmpt(cmpt)
{
	m_selection = spritesImpl->GetSpriteSelection();
	m_selection->Retain();

	m_pos.setInvalid();
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
		m_batch.insertToSpritesImpl(m_spritesImpl, m_pos, m_cmpt->isHorMirror(), m_cmpt->isVerMirror());
	else
		m_batch.insertToSpritesImpl(m_spritesImpl, m_pos, false, false);
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

	m_stage->Refresh();

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

	m_pos.setInvalid();
	m_batch.clear();

	return false;
}

void PasteSpriteOP::setMousePos(int x, int y)
{
	m_pos = m_stage->TransPosScrToProj(x, y);
	if (wxGetKeyState(WXK_SHIFT))
		fixPosOrthogonal();
}

void PasteSpriteOP::fixPosOrthogonal()
{
	const Vector& center = m_batch.getCenter();
	if (m_pos.isValid() && center.isValid())
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
	m_center.setInvalid();
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
		selection.Traverse(FetchAllVisitor<ISprite>(m_selected));
		computeCenter();
	}
}

void PasteSpriteOP::SpriteBatch::
insertToSpritesImpl(MultiSpritesImpl* spritesImpl, const Vector& pos,
					bool isHorMirror, bool isVerMirror)
{
	for (size_t i = 0, n = m_selected.size(); i < n; ++i)
	{
		ISprite* sprite = m_selected[i];
		Vector fixed = sprite->getPosition() - m_center;
		if (isHorMirror)
			fixed.x += (m_center.x - sprite->getPosition().x) * 2;
		if (isVerMirror)
			fixed.y += (m_center.y - sprite->getPosition().y) * 2;

//		spritesImpl->insertSprite(sprite->getSymbol(), fixed + pos);

		ISprite* newOne = sprite->Clone();
		newOne->setTransform(fixed + pos, newOne->getAngle());
		spritesImpl->InsertSprite(newOne);
		newOne->Release();
	}
}

void PasteSpriteOP::SpriteBatch::
draw(const Vector& pos, bool isHorMirror, bool isVerMirror) const
{
	if (!m_selected.empty() && pos.isValid())
	{
		const float xOffset = pos.x - m_center.x,
			yOffset = pos.y - m_center.y;

		for (size_t i = 0, n = m_selected.size(); i < n; ++i)
		{
			const Vector& pos = m_selected[i]->getPosition();

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
	m_center.setInvalid();
}

void PasteSpriteOP::SpriteBatch::
computeCenter()
{
	m_center.set(0, 0);
	for (size_t i = 0, n = m_selected.size(); i < n; ++i)
		m_center += m_selected[i]->getPosition();
	m_center /= m_selected.size();
}

} // d2d