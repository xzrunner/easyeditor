#include "StagePanel.h"
#include "StageCanvas.h"
#include "Context.h"
#include "ResizeCMPT.h"

namespace e9patch
{

StagePanel::StagePanel(wxWindow* parent,
					   wxTopLevelWindow* frame)
	: d2d::EditPanel(parent, frame)
	, d2d::MultiSpritesImpl(this)
	, m_patch(NULL)
{
	m_canvas = new StageCanvas(this);

	memset(m_sprites, 0, sizeof(int) * 9);

	SetDropTarget(new DragSymbolTarget());
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
	const float edge = Context::Instance()->EDGE;
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
	sprite->setTransform(d2d::Vector(edge*0.5f+edge*j, edge*0.5f+edge*i), 
		sprite->getAngle());

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
		m_patch = new d2d::Patch9Symbol;
		m_patch->composeFromSprites(m_sprites, Context::Instance()->width, 
			Context::Instance()->height);

		Context* context = Context::Instance();
		if (m_patch->type() == d2d::Patch9Symbol::e_3GridHor)
		{
			context->height = m_patch->getHeight();
			context->resizeCMPT->setValue(context->width, context->height);
		}
		else if (m_patch->type() == d2d::Patch9Symbol::e_3GridVer)
		{
			context->width = m_patch->getWidth();
			context->resizeCMPT->setValue(context->width, context->height);
		}
	}
}

bool StagePanel::isComplete() const
{
	if (m_sprites[1][0] && m_sprites[1][1] && m_sprites[1][2])
		return true;
	if (m_sprites[0][1] && m_sprites[1][1] && m_sprites[2][1])
		return true;

	for (size_t i = 1; i < 3; ++i)
		for (size_t j = 0; j < 3; ++j)
			if (!m_sprites[i][j]) return false;
	return true;

	for (size_t i = 0; i < 3; ++i)
		for (size_t j = 0; j < 3; ++j)
			if (!m_sprites[i][j]) return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// class StagePanel::DragSymbolTarget
//////////////////////////////////////////////////////////////////////////

bool StagePanel::DragSymbolTarget::
OnDropText(wxCoord x, wxCoord y, const wxString& data)
{
	wxString sType = data.substr(0, data.find(","));
	wxString sIndex = data.substr(data.find(",") + 1);

	long index;
	sIndex.ToLong(&index);

	StagePanel* stage = static_cast<StagePanel*>(Context::Instance()->stage);
	d2d::LibraryPanel* library = static_cast<d2d::LibraryPanel*>(Context::Instance()->library);

	d2d::Vector pos = stage->transPosScreenToProject(x, y);

	d2d::ISymbol* symbol = library->getSymbol(index);
	d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
	sprite->translate(pos);

	stage->insertSprite(sprite);

	return true;
}
} // e9patch