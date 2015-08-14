#include "StageCanvas.h"
#include "StagePanel.h"
#include "Context.h"

using namespace edb;

StageCanvas::StageCanvas(StagePanel* editPanel)
	: d2d::OrthoCanvas(editPanel, editPanel->GetStageImpl())
	, m_stage_panel(editPanel)
	, m_batch(100, d2d::SpriteBatch::USAGE_STATIC)
{
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::OnDrawSprites() const
{
	m_stage_panel->TraverseShapes(d2d::DrawShapesVisitor(d2d::Rect()), d2d::DT_VISIBLE);

	// fixme
//	editPanel->traverseSprites(d2d::DrawSpritesVisitor(m_batch), d2d::DT_VISIBLE);

	drawConnection();
	m_stage_panel->DrawEditOP();
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
			d2d::PrimitiveDraw::drawLine(from->GetPosition(), to->GetPosition(), 
				d2d::Colorf(0.8f, 0.8f, 0.8f));
		}
	}
}