#include "StageCanvas.h"
#include "StagePanel.h"
#include "Context.h"

#include <ee/DrawShapesVisitor.h>
#include <ee/EE_RVG.h>
#include <ee/Sprite.h>

namespace edb
{

StageCanvas::StageCanvas(StagePanel* editPanel)
	: ee::CameraCanvas(editPanel, editPanel->GetStageImpl())
	, m_stage_panel(editPanel)
{
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::OnDrawSprites() const
{
	m_stage_panel->TraverseShapes(ee::DrawShapesVisitor(ee::Rect()), ee::DT_VISIBLE);

	// fixme
//	editPanel->traverseSprites(ee::DrawSpritesVisitor(m_batch), ee::DT_VISIBLE);

	drawConnection();
	m_stage_panel->DrawEditOP();
}

void StageCanvas::drawConnection() const
{
	std::map<ee::Sprite*, Node*>& connection
		= Context::Instance()->stage->m_graphics.connection;
	std::map<ee::Sprite*, Node*>::iterator itr = connection.begin();
	for ( ; itr != connection.end(); ++itr)
	{
		ee::Sprite* from = itr->first;
		for (size_t i = 0, n = itr->second->out.size(); i < n; ++i)
		{
			ee::Sprite* to = itr->second->out[i];
			ee::RVG::Color(ee::Colorf(0.8f, 0.8f, 0.8f));
			ee::RVG::Line(from->GetPosition(), to->GetPosition());
		}
	}
}

}