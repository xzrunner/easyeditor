#include "StagePanel.h"
#include "StageCanvas.h"
#include "ResizeCMPT.h"
#include "ToolbarPanel.h"
#include "config.h"

#include <easyscale9.h>

namespace escale9
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::MultiSpritesImpl(this)
	, m_patch(NULL)
	, m_library(library)
	, m_toolbar(NULL)
{
	m_canvas = new StageCanvas(this);

	memset(m_sprites, 0, sizeof(int) * 9);

	SetDropTarget(new d2d::StageDropTarget(static_cast<d2d::Frame*>(frame), this, this, library));
}

StagePanel::~StagePanel()
{
	clear();
}

void StagePanel::clear()
{
	EditPanel::clear();
	clearSprites();
}

void StagePanel::traverseSprites(d2d::IVisitor& visitor, d2d::TraverseType type/* = d2d::e_allExisting*/, 
								 bool order/* = true*/) const
{
	for (size_t i = 0; i < 3; ++i)
		for (size_t j = 0; j < 3; ++j)
		{
			if (!m_sprites[i][j]) continue;

			bool hasNext;
			visitor.visit(m_sprites[i][j], hasNext);
			if (!hasNext) return;
		}
}

void StagePanel::removeSprite(d2d::ISprite* sprite)
{
	for (size_t i = 0; i < 3; ++i)
		for (size_t j = 0; j < 3; ++j)
		{
			if (m_sprites[i][j] == sprite)
			{
				m_sprites[i][j] = NULL;
				sprite->release();
				return;
			}
		}
}

void StagePanel::insertSprite(d2d::ISprite* sprite)
{
	const float edge = EDGE;
	const d2d::Vector& pos = sprite->getPosition();

	int i, j;

	if (pos.y < edge) i = 0;
	else if (pos.y >= edge && pos.y < edge * 2) i = 1;
	else if (pos.y > edge * 2) i = 2;

	if (pos.x < edge) j = 0;
	else if (pos.x >= edge && pos.x < edge * 2) j = 1;
	else if (pos.x > edge * 2) j = 2;

	if (m_sprites[i][j] && m_sprites[i][j] != sprite)
		m_sprites[i][j]->release();
	m_sprites[i][j] = sprite;
	d2d::Vector center(edge*0.5f+edge*j, edge*0.5f+edge*i);
	sprite->setTransform(center, sprite->getAngle());

	rebuildPatchSymbol();

	Refresh();
}

void StagePanel::clearSprites()
{
	for (size_t i = 0; i < 3; ++i)
		for (size_t j = 0; j < 3; ++j)
		{
			if (!m_sprites[i][j]) continue;
			m_sprites[i][j]->release();
		}
	memset(m_sprites, 0, sizeof(int) * 9);

	delete m_patch, m_patch = NULL;
}

void StagePanel::resetSpriteOrder(d2d::ISprite* sprite, bool up)
{
}

void StagePanel::rebuildPatchSymbol()
{
	if (isComplete())
	{
		if (m_patch) delete m_patch;
		m_patch = new Symbol;

		float width = m_toolbar->getWidth(),
			  height = m_toolbar->getHeight();
		m_patch->composeFromSprites(m_sprites, width, height);
		if (m_patch->type() == Symbol::e_3GridHor)
		{
			height = m_patch->getSize().yLength();
			m_toolbar->setSize(width, height);
		}
		else if (m_patch->type() == Symbol::e_3GridVer)
		{
			width = m_patch->getSize().xLength();
			m_toolbar->setSize(width, height);
		}
	}
}

void StagePanel::setToolbarPanel(ToolbarPanel* toolbar)
{
	m_toolbar = toolbar;
	static_cast<StageCanvas*>(m_canvas)->setToolbarPanel(toolbar);
}

bool StagePanel::isComplete() const
{
	// e_3GridHor
	if (m_sprites[1][0] && m_sprites[1][1] && m_sprites[1][2])
		return true;

	// e_3GridVer
	if (m_sprites[0][1] && m_sprites[1][1] && m_sprites[2][1])
		return true;

	// e_6GridUpper
	if (m_sprites[1][0] && m_sprites[1][1] && m_sprites[1][2] &&
		m_sprites[2][0] && m_sprites[2][1] && m_sprites[2][2])
		return true;

	// e_9Grid
	if (m_sprites[0][0] && m_sprites[0][1] && m_sprites[0][2] &&
		m_sprites[1][0] && m_sprites[1][1] && m_sprites[1][2] &&
		m_sprites[2][0] && m_sprites[2][1] && m_sprites[2][2])
		return true;

	// e_9GridHollow
	if (m_sprites[0][0] && m_sprites[0][1] && m_sprites[0][2] &&
		m_sprites[1][0] &&					  m_sprites[1][2] &&
		m_sprites[2][0] && m_sprites[2][1] && m_sprites[2][2])
		return true;

	return false;
}

} // escale9