#include "StageCanvas.h"
#include "StagePanel.h"
#include "Context.h"

using namespace edb;

StageCanvas::StageCanvas(StagePanel* editPanel)
	: d2d::OrthoCanvas(editPanel)
	, m_batch(100, d2d::SpriteBatch::USAGE_STATIC)
{
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::OnDraw()
{
	StagePanel* editPanel = static_cast<StagePanel*>(m_stage);
	editPanel->traverseShapes(d2d::DrawShapesVisitor(), d2d::DT_VISIBLE);
	editPanel->traverseSprites(d2d::DrawSpritesVisitor(m_batch), d2d::DT_VISIBLE);
	drawConnection();
	editPanel->drawEditTemp();
}

void StageCanvas::drawConnection() const
{
	std::map<d2d::ISprite*, Node*>& connection
		= Context::Instance()->stage->m_graphics.connection;
	std::map<d2d::ISprite*, Node*>::iterator itr = connection.begin();
	for ( ; itr != connection.end(); ++itr)
	{
		d2d::ISprite* from = itr->first;
		for (size_t i = 0, n = itr->second->out.size(); i < n; ++i)
		{
			d2d::ISprite* to = itr->second->out[i];
			d2d::PrimitiveDraw::drawLine(from->getPosition(), to->getPosition(), 
				d2d::Colorf(0.8f, 0.8f, 0.8f));
		}
	}
}