#include "PasteSymbolTileOP.h"
#include "SpriteFactory.h"
#include "Sprite.h"
#include "MultiSpritesImpl.h"
#include "LibraryPanel.h"
#include "StageCanvas.h"
#include "EditPanelImpl.h"
#include "SpriteRenderer.h"
#include "sprite_msg.h"
#include "panel_msg.h"

#include <SM_Calc.h>

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
	auto sym = m_library->GetSymbol();
	if (sym) 
	{
		if (!m_bCaptured)
			m_pos = m_stage->TransPosScrToProj(x, y);

		auto spr = SpriteFactory::Instance()->CreateRoot(sym);
		spr->Translate(m_pos);
		spr->Rotate(m_rotate);
		InsertSpriteSJ::Instance()->Insert(spr);
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

	NearestQueryVisitor visitor(m_pos);
	m_spritesImpl->TraverseSprites(visitor, DT_EDITABLE);
	auto spr = visitor.GetResult();
	if (!spr) return false;

	const sm::vec2& capture = spr->GetPosition();
	sm::vec2 offset = m_cmpt->GetOffset();
	const float dis = offset.Length() * 0.5f;
	do
	{
		sm::vec2 newPos = sm::vec2(capture.x + offset.x, capture.y + offset.y);
		if (sm::dis_pos_to_pos(m_pos, newPos) < dis)
		{
			m_bCaptured = true;
			m_pos = newPos;
			break;
		}

		newPos = sm::vec2(capture.x + offset.x, capture.y - offset.y);
		if (sm::dis_pos_to_pos(m_pos, newPos) < dis)
		{
			m_bCaptured = true;
			m_pos = newPos;
			break;
		}

		newPos = sm::vec2(capture.x - offset.x, capture.y + offset.y);
		if (sm::dis_pos_to_pos(m_pos, newPos) < dis)
		{
			m_bCaptured = true;
			m_pos = newPos;
			break;
		}

		newPos = sm::vec2(capture.x - offset.x, capture.y - offset.y);
		if (sm::dis_pos_to_pos(m_pos, newPos) < dis)
		{
			m_bCaptured = true;
			m_pos = newPos;
			break;
		}

		newPos = sm::vec2(capture.x, capture.y - offset.y);
		if (sm::dis_pos_to_pos(m_pos, newPos) < dis)
		{
			m_bCaptured = true;
			m_pos = newPos;
			break;
		}

		newPos = sm::vec2(capture.x, capture.y + offset.y);
		if (sm::dis_pos_to_pos(m_pos, newPos) < dis)
		{
			m_bCaptured = true;
			m_pos = newPos;
			break;
		}

		newPos = sm::vec2(capture.x - offset.x, capture.y);
		if (sm::dis_pos_to_pos(m_pos, newPos) < dis)
		{
			m_bCaptured = true;
			m_pos = newPos;
			break;
		}

		newPos = sm::vec2(capture.x + offset.x, capture.y);
		if (sm::dis_pos_to_pos(m_pos, newPos) < dis)
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

	auto sym = m_library->GetSymbol();
	if (sym && m_pos.IsValid())
	{
		if (m_scale) {
			ee::SpriteRenderer::Instance()->Draw(sym.get(), s2::RenderParams(), m_pos, m_rotate, sm::vec2(*m_scale, *m_scale));
		} else {
			ee::SpriteRenderer::Instance()->Draw(sym.get(), s2::RenderParams(), m_pos, m_rotate);
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
// class PasteSymbolTileOP::NearestQueryVisitor
//////////////////////////////////////////////////////////////////////////

PasteSymbolTileOP::NearestQueryVisitor::
NearestQueryVisitor(const sm::vec2& pos)
	: m_pos(pos)
	, m_dis(FLT_MAX)
{
}

void PasteSymbolTileOP::NearestQueryVisitor::
Visit(const SprPtr& spr, bool& next)
{
	const float dis = sm::dis_pos_to_pos(spr->GetPosition(), m_pos);
	if (dis < m_dis)
	{
		m_result = spr;
		m_dis = dis;
	}

	next = true;
}
}