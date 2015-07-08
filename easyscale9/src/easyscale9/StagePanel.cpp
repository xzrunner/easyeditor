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
	, m_symbol(NULL)
	, m_library(library)
	, m_toolbar(NULL)
{
	m_canvas = new StageCanvas(this);

	memset(m_sprites, 0, sizeof(int) * 9);

	SetDropTarget(new d2d::StageDropTarget(this, this, library));
}

StagePanel::~StagePanel()
{
	Clear();
}

void StagePanel::Clear()
{
	EditPanel::Clear();
	ClearAllSprite();
}

bool StagePanel::InsertSprite(d2d::ISprite* sprite)
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
		m_sprites[i][j]->Release();
	sprite->Retain();
	m_sprites[i][j] = sprite;
	d2d::Vector center(edge*0.5f+edge*j, edge*0.5f+edge*i);
	sprite->setTransform(center, sprite->getAngle());

	rebuildPatchSymbol();

	return true;
}

bool StagePanel::RemoveSprite(d2d::ISprite* sprite)
{
	for (size_t i = 0; i < 3; ++i) 
	{
		for (size_t j = 0; j < 3; ++j) 
		{
			if (m_sprites[i][j] == sprite)
			{
				m_sprites[i][j] = NULL;
				sprite->Release();
				return true;
			}
		}
	}
	return false;
}

bool StagePanel::ClearAllSprite()
{
	for (size_t i = 0; i < 3; ++i) {
		for (size_t j = 0; j < 3; ++j)
		{
			if (!m_sprites[i][j]) continue;
			m_sprites[i][j]->Release();
		}
	}
	memset(m_sprites, 0, sizeof(int) * 9);

	delete m_symbol, m_symbol = NULL;

	return true;
}

void StagePanel::TraverseSprites(d2d::IVisitor& visitor, d2d::DataTraverseType type/* = d2d::e_allExisting*/, 
								 bool order/* = true*/) const
{
	for (size_t i = 0; i < 3; ++i) 
	{
		for (size_t j = 0; j < 3; ++j)
		{
			if (!m_sprites[i][j]) continue;

			bool hasNext;
			visitor.Visit(m_sprites[i][j], hasNext);
			if (!hasNext) return;
		}
	}
}

void StagePanel::rebuildPatchSymbol()
{
	Scale9Type type = Scale9Data::CheckType(m_sprites);
	if (type == e_null) {
		return;
	}

	if (m_symbol) delete m_symbol;
	m_symbol = new Symbol;

	float width = m_toolbar->getWidth(),
		  height = m_toolbar->getHeight();

	if (type == e_3GridHor) {
		height = m_sprites[1][1]->getSymbol().GetSize().yLength();
	} else if (type == e_3GridVer) {
		width = m_sprites[1][1]->getSymbol().GetSize().xLength();
	}

	m_symbol->ComposeFromSprites(m_sprites, width, height);

	m_toolbar->setSize(width, height);
}

void StagePanel::setToolbarPanel(ToolbarPanel* toolbar)
{
	m_toolbar = toolbar;
	static_cast<StageCanvas*>(m_canvas)->setToolbarPanel(toolbar);
}

} // escale9