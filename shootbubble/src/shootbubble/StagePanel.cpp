#include "StagePanel.h"
#include "StageCanvas.h"
#include "Context.h"

using namespace shootbubble;

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame)
	: EditPanel(parent, frame)
	, SpritesPanelImpl(this, Context::Instance()->library)
{
	m_canvas = new StageCanvas(this);
}

void StagePanel::clear()
{
	EditPanel::clear();
	SpritesPanelImpl::clearSprites();
}

void StagePanel::insertSprite(ee::ISprite* sprite)
{
	sprite->setTransform(fixSpriteLocation(sprite->getPosition()), 0);
	if (sprite->getPosition().isValid())
		ee::SpritesPanelImpl::insertSprite(sprite);
}

void StagePanel::transCoordsToGridPos(const ee::Vector& pos, int& row, int& col) const
{
	Context* context = Context::Instance();

	float hEdge, posOffset, posOffset1, rowOffset;
	computeParams(hEdge, posOffset, posOffset1, rowOffset);

	row = (int) ((-pos.y - posOffset1) / rowOffset + 0.5f);
	assert(fabs(-posOffset1 - rowOffset * row - pos.y) < ee::LARGE_EPSILON);

	if (row % 2)
	{
		col = (int) (pos.x / context->edge + 0.5) - 1;
		assert(fabs(context->edge * (col + 1) - pos.x) < ee::LARGE_EPSILON);
	}
	else
	{
		col = (int) ((pos.x + hEdge) / context->edge + 0.5f) - 1;
		assert(fabs(hEdge + context->edge * col - pos.x) < ee::LARGE_EPSILON);
	}
}

void StagePanel::transGridPosToCoords(int row, int col, ee::Vector& pos) const
{
	float hEdge, posOffset, posOffset1, rowOffset;
	computeParams(hEdge, posOffset, posOffset1, rowOffset);

	pos.y = -posOffset1 - rowOffset * row;
	if (row % 2)
		pos.x = (col + 1) * Context::Instance()->edge;
	else
		pos.x = hEdge + Context::Instance()->edge * col;
}

void StagePanel::computeParams(float& hEdge, float& posOffset, 
							   float& posOffset1, float& rowOffset) const
{
	hEdge = Context::Instance()->edge * 0.5f;
	posOffset = hEdge / 1.732051;
	posOffset1 = posOffset * 2;
	rowOffset = hEdge * 1.732051;
}

void StagePanel::updateAllSpritesLocation()
{
	std::vector<ee::ISprite*> sprites;
	traverseSprites(ee::FetchAllVisitor<ee::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		sprites[i]->setTransform(fixSpriteLocation(sprites[i]->getPosition()), 0);
}

ee::Vector StagePanel::fixSpriteLocation(const ee::Vector& pos) const
{
	Context* context = Context::Instance();

	ee::Vector fixed;

	float hEdge, posOffset, posOffset1, rowOffset;
	computeParams(hEdge, posOffset, posOffset1, rowOffset);

	int width = context->edge * context->col,
		height = posOffset1 + rowOffset * (context->row - 1) + posOffset1;
	if (pos.x < 0 || pos.x > width ||
		pos.y > 0 || pos.y < -height)
	{
		fixed.setInvalid();
	}
	else
	{
		fixed.x = hEdge + (int) (pos.x / context->edge) * context->edge;
		if (-pos.y < posOffset1)
		{
			fixed.y = -posOffset1;
		}
		else
		{
			int row = (int) ((-pos.y - posOffset1) / rowOffset);

			ee::Vector c0, c1;
			c0.y = -posOffset1 - rowOffset * row;
			c1.y = -posOffset1 - rowOffset * (row + 1);
			if (row % 2)
			{
				c0.x = context->edge + (int)((pos.x - hEdge) / context->edge) * Context::Instance()->edge;
				c1.x = fixed.x;
			}
			else
			{
				c0.x = fixed.x;
				c1.x = context->edge + (int)((pos.x - hEdge) / context->edge) * Context::Instance()->edge;
			}

			if (ee::Math::getDistanceSquare(pos, c0) < ee::Math::getDistanceSquare(pos, c1))
				fixed = c0;
			else
				fixed = c1;
		}
	}

	return fixed;
}
