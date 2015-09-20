#include "StagePanel.h"
#include "StageCanvas.h"
#include "ResizeCMPT.h"
#include "ToolbarPanel.h"
#include "ComposeGrids.h"

#include <easyscale9.h>

namespace escale9
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::MultiSpritesImpl(GetStageImpl())
	, m_symbol(NULL)
	, m_library(library)
	, m_toolbar(NULL)
{
	SetCanvas(new StageCanvas(this));

	memset(m_sprites, 0, sizeof(int) * 9);

	SetDropTarget(new d2d::StageDropTarget(this, GetStageImpl(), this, library));
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

bool StagePanel::InsertSprite(d2d::ISprite* sprite, int idx)
{
	d2d::MultiSpritesImpl::InsertSprite(sprite);

	const d2d::Vector& pos = sprite->GetPosition();
	int col, row;
	ComposeGrids::Query(pos, &col, &row);
	if (col == -1 || row == -1) {
		return false;
	}

	if (m_sprites[row][col] && m_sprites[row][col] != sprite) {
		m_sprites[row][col]->Release();
	}
	sprite->Retain();
	m_sprites[row][col] = sprite;

	sprite->SetTransform(ComposeGrids::GetGridCenter(col, row), 
		sprite->GetAngle());

	rebuildPatchSymbol();

	SetCanvasDirty();

	return true;
}

bool StagePanel::RemoveSprite(d2d::ISprite* sprite)
{
	d2d::MultiSpritesImpl::RemoveSprite(sprite);

	for (size_t i = 0; i < 3; ++i) 
	{
		for (size_t j = 0; j < 3; ++j) 
		{
			if (m_sprites[i][j] == sprite)
			{
				m_sprites[i][j] = NULL;
				sprite->Release();
				SetCanvasDirty();
				return true;
			}
		}
	}
	return false;
}

bool StagePanel::ClearAllSprite()
{
	d2d::MultiSpritesImpl::ClearAllSprite();

	for (size_t i = 0; i < 3; ++i) {
		for (size_t j = 0; j < 3; ++j)
		{
			if (!m_sprites[i][j]) continue;
			m_sprites[i][j]->Release();
		}
	}
	memset(m_sprites, 0, sizeof(int) * 9);

	delete m_symbol, m_symbol = NULL;

	SetCanvasDirty();

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
		height = m_sprites[1][1]->GetSymbol().GetSize().yLength();
	} else if (type == e_3GridVer) {
		width = m_sprites[1][1]->GetSymbol().GetSize().xLength();
	}

	m_symbol->ComposeFromSprites(m_sprites, width, height);

	m_toolbar->setSize(width, height);
}

void StagePanel::setToolbarPanel(ToolbarPanel* toolbar)
{
	m_toolbar = toolbar;
	static_cast<StageCanvas*>(GetCanvas())->setToolbarPanel(toolbar);
}

} // escale9