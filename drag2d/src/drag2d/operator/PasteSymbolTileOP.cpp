#include "PasteSymbolTileOP.h"

#include "common/Math.h"
#include "dataset/SpriteFactory.h"
#include "dataset/ISprite.h"
#include "view/MultiSpritesImpl.h"
#include "view/LibraryPanel.h"
#include "view/IStageCanvas.h"
#include "render/SpriteRenderer.h"

namespace d2d
{

PasteSymbolTileOP::PasteSymbolTileOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl,
									 LibraryPanel* libraryPanel, PasteSymbolOffsetCMPT<PasteSymbolTileOP>* cmpt)
	: PasteSymbolOP(editPanel, spritesImpl, libraryPanel)
	, m_cmpt(cmpt)
	, m_bCaptured(false)
	, m_rotate(0)
{
}

bool PasteSymbolTileOP::OnMouseLeftDown(int x, int y)
{
	ISymbol* symbol = m_libraryPanel->GetSymbol();
	if (symbol) 
	{
		if (!m_bCaptured)
			m_pos = m_stage->TransPosScrToProj(x, y);

		ISprite* sprite = SpriteFactory::Instance()->create(symbol);
		sprite->Translate(m_pos);
		sprite->Rotate(m_rotate);
		m_panelImpl->InsertSprite(sprite);
		sprite->Release();
	}

	return false;
}

bool PasteSymbolTileOP::OnMouseRightDown(int x, int y)
{
	if (PasteSymbolOP::OnMouseRightDown(x, y)) return true;

	m_rotate += d2d::PI * 0.5f;

	return false;
}

bool PasteSymbolTileOP::OnMouseMove(int x, int y)
{
	if (ZoomViewOP::OnMouseMove(x, y)) return true;

	m_bCaptured = false;
	m_pos = m_stage->TransPosScrToProj(x, y);

	Vector offset = m_cmpt->getOffset();
	const float dis = offset.length() * 0.5f;
	ISprite* sprite = NULL;
	m_panelImpl->TraverseSprites(NearestQueryVisitor(m_pos, &sprite), DT_EDITABLE);
	if (!sprite) return false;

	const d2d::Vector& capture = sprite->GetPosition();
	if (capture.isValid())
	{
		Vector offset = m_cmpt->getOffset();
		const float dis = offset.length() * 0.5f;
		do
		{
			Vector newPos = d2d::Vector(capture.x + offset.x, capture.y + offset.y);
			if (Math::getDistance(m_pos, newPos) < dis)
			{
				m_bCaptured = true;
				m_pos = newPos;
				break;
			}

			newPos = d2d::Vector(capture.x + offset.x, capture.y - offset.y);
			if (Math::getDistance(m_pos, newPos) < dis)
			{
				m_bCaptured = true;
				m_pos = newPos;
				break;
			}

			newPos = d2d::Vector(capture.x - offset.x, capture.y + offset.y);
			if (Math::getDistance(m_pos, newPos) < dis)
			{
				m_bCaptured = true;
				m_pos = newPos;
				break;
			}

			newPos = d2d::Vector(capture.x - offset.x, capture.y - offset.y);
			if (Math::getDistance(m_pos, newPos) < dis)
			{
				m_bCaptured = true;
				m_pos = newPos;
				break;
			}

			newPos = d2d::Vector(capture.x, capture.y - offset.y);
			if (Math::getDistance(m_pos, newPos) < dis)
			{
				m_bCaptured = true;
				m_pos = newPos;
				break;
			}

			newPos = d2d::Vector(capture.x, capture.y + offset.y);
			if (Math::getDistance(m_pos, newPos) < dis)
			{
				m_bCaptured = true;
				m_pos = newPos;
				break;
			}

			newPos = d2d::Vector(capture.x - offset.x, capture.y);
			if (Math::getDistance(m_pos, newPos) < dis)
			{
				m_bCaptured = true;
				m_pos = newPos;
				break;
			}

			newPos = d2d::Vector(capture.x + offset.x, capture.y);
			if (Math::getDistance(m_pos, newPos) < dis)
			{
				m_bCaptured = true;
				m_pos = newPos;
				break;
			}

		} while (0);
	}
	m_stage->SetCanvasDirty();

	return false;
}

bool PasteSymbolTileOP::OnDraw() const
{
	if (ZoomViewOP::OnDraw()) return true;

	ISymbol* symbol = m_libraryPanel->GetSymbol();
	if (symbol && m_pos.isValid())
	{
		SpriteRenderer* rd = SpriteRenderer::Instance();
		if (m_pScale) {
			rd->Draw(symbol, Matrix(), m_pos, m_rotate, *m_pScale);
		} else {
			rd->Draw(symbol, Matrix(), m_pos, m_rotate);
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
// class PasteSymbolTileOP::NearestQueryVisitor
//////////////////////////////////////////////////////////////////////////

PasteSymbolTileOP::NearestQueryVisitor::
NearestQueryVisitor(const Vector& pos, ISprite** ret)
	: m_pos(pos)
	, m_dis(FLT_MAX)
	, m_result(ret)
{
}

void PasteSymbolTileOP::NearestQueryVisitor::
Visit(Object* object, bool& bFetchNext)
{
	ISprite* sprite = static_cast<ISprite*>(object);

	const float dis = Math::getDistance(sprite->GetPosition(), m_pos);
	if (dis < m_dis)
	{
		*m_result = sprite;
		m_dis = dis;
	}

	bFetchNext = true;
}
} // d2d