#include "StagePanel.h"
#include "StageCanvas.h"

namespace sg
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   d2d::LibraryPanel* library)
	: EditPanel(parent, frame)
	, SpritesPanelImpl(this, library)
{
	m_canvas = new StageCanvas(this);

	m_row = m_col = m_edge = 0;
}

void StagePanel::clear()
{
	EditPanel::clear();
	SpritesPanelImpl::clearSprites();
}

void StagePanel::insertSprite(d2d::ISprite* sprite)
{
	sprite->setTransform(fixSpriteLocation(sprite->getPosition()), 0);
	if (sprite->getPosition().isValid())
		d2d::SpritesPanelImpl::insertSprite(sprite);
}

void StagePanel::transCoordsToGridPos(const d2d::Vector& pos, int& row, int& col) const
{
	col = std::max(0.0f, std::min(pos.x / m_edge, (float)(m_col - 1)));
	row = std::max(0.0f, std::min(pos.y / m_edge, (float)(m_row - 1)));
}

void StagePanel::transGridPosToCoords(int row, int col, d2d::Vector& pos) const
{
	pos.x = m_edge * col + m_edge * 0.5f;
	pos.y = m_edge * row + m_edge * 0.5f;
}

void StagePanel::updateAllSpritesLocation()
{
	std::vector<d2d::ISprite*> sprites;
	traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		sprites[i]->setTransform(fixSpriteLocation(sprites[i]->getPosition()), 0);
}

d2d::Vector StagePanel::fixSpriteLocation(const d2d::Vector& pos) const
{
	int row, col;
	transCoordsToGridPos(pos, row, col);

	d2d::Vector ret;
	transGridPosToCoords(row, col, ret);
	return ret;
}

}