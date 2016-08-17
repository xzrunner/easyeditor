#include "PasteSymbolTileOP.h"
#include "Math2D.h"
#include "SpriteFactory.h"
#include "Sprite.h"
#include "MultiSpritesImpl.h"
#include "LibraryPanel.h"
#include "StageCanvas.h"
#include "EditPanelImpl.h"
#include "SpriteRenderer.h"
#include "sprite_msg.h"
#include "panel_msg.h"

namespace ee
{

PasteSymbolTileOP::PasteSymbolTileOP(wxWindow* wnd, EditPanelImpl* stage, MultiSpritesImpl* sprites_impl,
									 LibraryPanel* library, PasteSymbolOffsetCMPT<PasteSymbolTileOP>* cmpt)
	: PasteSymbolOP(wnd, stage, library)
	, m_spritesImpl(sprites_impl)
	, m_cmpt(cmpt)
	, m_bCaptured(false)
	, m_rotate(0)
{
}

bool PasteSymbolTileOP::OnMouseLeftDown(int x, int y)
{
	Symbol* sym = m_library->GetSymbol();
	if (sym) 
	{
		if (!m_bCaptured)
			m_pos = m_stage->TransPosScrToProj(x, y);

		Sprite* spr = SpriteFactory::Instance()->Create(sym);
		spr->Translate(m_pos);
		spr->Rotate(m_rotate);
		InsertSpriteSJ::Instance()->Insert(spr);
		spr->RemoveReference();
	}

	return false;
}

bool PasteSymbolTileOP::OnMouseRightDown(int x, int y)
{
	if (PasteSymbolOP::OnMouseRightDown(x, y)) return true;

	m_rotate += SM_PI * 0.5f;

	return false;
}

bool PasteSymbolTileOP::OnMouseMove(int x, int y)
{
	if (ZoomViewOP::OnMouseMove(x, y)) return true;

	m_bCaptured = false;
	m_pos = m_stage->TransPosScrToProj(x, y);

	Sprite* spr = NULL;
	m_spritesImpl->TraverseSprites(NearestQueryVisitor(m_pos, &spr), DT_EDITABLE);
	if (!spr) return false;

	const sm::vec2& capture = spr->GetPosition();
	sm::vec2 offset = m_cmpt->GetOffset();
	const float dis = offset.Length() * 0.5f;
	do
	{
		sm::vec2 newPos = sm::vec2(capture.x + offset.x, capture.y + offset.y);
		if (Math2D::GetDistance(m_pos, newPos) < dis)
		{
			m_bCaptured = true;
			m_pos = newPos;
			break;
		}

		newPos = sm::vec2(capture.x + offset.x, capture.y - offset.y);
		if (Math2D::GetDistance(m_pos, newPos) < dis)
		{
			m_bCaptured = true;
			m_pos = newPos;
			break;
		}

		newPos = sm::vec2(capture.x - offset.x, capture.y + offset.y);
		if (Math2D::GetDistance(m_pos, newPos) < dis)
		{
			m_bCaptured = true;
			m_pos = newPos;
			break;
		}

		newPos = sm::vec2(capture.x - offset.x, capture.y - offset.y);
		if (Math2D::GetDistance(m_pos, newPos) < dis)
		{
			m_bCaptured = true;
			m_pos = newPos;
			break;
		}

		newPos = sm::vec2(capture.x, capture.y - offset.y);
		if (Math2D::GetDistance(m_pos, newPos) < dis)
		{
			m_bCaptured = true;
			m_pos = newPos;
			break;
		}

		newPos = sm::vec2(capture.x, capture.y + offset.y);
		if (Math2D::GetDistance(m_pos, newPos) < dis)
		{
			m_bCaptured = true;
			m_pos = newPos;
			break;
		}

		newPos = sm::vec2(capture.x - offset.x, capture.y);
		if (Math2D::GetDistance(m_pos, newPos) < dis)
		{
			m_bCaptured = true;
			m_pos = newPos;
			break;
		}

		newPos = sm::vec2(capture.x + offset.x, capture.y);
		if (Math2D::GetDistance(m_pos, newPos) < dis)
		{
			m_bCaptured = true;
			m_pos = newPos;
			break;
		}

	} while (0);
	SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool PasteSymbolTileOP::OnDraw() const
{
	if (ZoomViewOP::OnDraw()) return true;

	Symbol* sym = m_library->GetSymbol();
	if (sym && m_pos.IsValid())
	{
		if (m_scale) {
			SpriteRenderer::Draw(sym, s2::RenderParams(), m_pos, m_rotate, *m_scale);
		} else {
			SpriteRenderer::Draw(sym, s2::RenderParams(), m_pos, m_rotate);
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
// class PasteSymbolTileOP::NearestQueryVisitor
//////////////////////////////////////////////////////////////////////////

PasteSymbolTileOP::NearestQueryVisitor::
NearestQueryVisitor(const sm::vec2& pos, Sprite** ret)
	: m_pos(pos)
	, m_dis(FLT_MAX)
	, m_result(ret)
{
}

void PasteSymbolTileOP::NearestQueryVisitor::
Visit(Sprite* spr, bool& next)
{
	const float dis = Math2D::GetDistance(spr->GetPosition(), m_pos);
	if (dis < m_dis)
	{
		*m_result = spr;
		m_dis = dis;
	}

	next = true;
}
}