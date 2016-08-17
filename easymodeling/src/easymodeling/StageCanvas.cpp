#include "StageCanvas.h"
#include "StagePanel.h"
#include "DrawUtils.h"

#include <ee/EditPanelImpl.h>
#include <ee/FetchAllVisitor.h>
#include <ee/SpriteRenderer.h>
#include <ee/Sprite.h>

#include <sprite2/S2_RVG.h>

namespace emodeling
{

StageCanvas::StageCanvas(StagePanel* editPanel)
	: ee::CameraCanvas(editPanel, editPanel->GetStageImpl())
	, m_stage_panel(editPanel)
{
	m_bg_color.FromFloat(0, 0, 0, 1);
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::OnDrawSprites() const
{
	DrawGuideLines();

	DrawSprites();
	m_stage_panel->traverseJoints(DrawJointVisitor());
	m_stage->DrawEditOP();
}

void StageCanvas::DrawGuideLines() const
{
	DrawCrossLine();
}

void StageCanvas::DrawSprites() const
{
	std::vector<ee::Sprite*> sprs;
	m_stage_panel->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprs));
	for (size_t i = 0, n = sprs.size(); i < n; ++i)
	{
		ee::SpriteRenderer::Draw(sprs[i]);
		DrawUtils::DrawBody(static_cast<Body*>(sprs[i]->GetUserData()), DrawUtils::e_default);
	}
}

void StageCanvas::DrawCrossLine() const
{
	const float halfEdge = 1000.0f;
	std::vector<sm::vec2> vertices(4);
	vertices[0].x = -halfEdge;
	vertices[1].x = halfEdge;
	vertices[2].y = -halfEdge;
	vertices[3].y = halfEdge;
	s2::RVG::SetColor(s2::Color(0.7f, 0.9f, 0.7f));
	s2::RVG::LineWidth(1);
	s2::RVG::Lines(vertices);
	s2::RVG::LineWidth(2);
}

void StageCanvas::DrawLines() const
{
 	const float halfEdge = 100.0f;
	// green
	{
		std::vector<sm::vec2> vertices;
		size_t size = halfEdge * 2 * 2 * 2;
		vertices.reserve(size);
		for (float x = -halfEdge; x <= halfEdge; x += 1.0f)
		{
			vertices.push_back(sm::vec2(x, -halfEdge));
			vertices.push_back(sm::vec2(x, halfEdge));
		}
		for (float y = -halfEdge; y <= halfEdge; y += 1.0f)
		{
			vertices.push_back(sm::vec2(-halfEdge, y));
			vertices.push_back(sm::vec2(halfEdge, y));
		}
		s2::RVG::SetColor(s2::Color(0.7f, 0.9f, 0.7f));
		s2::RVG::LineWidth(1);
		s2::RVG::Lines(vertices);
		s2::RVG::LineWidth(2);
	}
	// red
	{
		std::vector<sm::vec2> vertices;
		size_t size = halfEdge * 0.1f * 2 * 2 * 2;
		vertices.reserve(size);
		for (float x = -halfEdge; x <= halfEdge; x += 10)
		{
			vertices.push_back(sm::vec2(x, -halfEdge));
			vertices.push_back(sm::vec2(x, halfEdge));
		}
		for (float y = -halfEdge; y <= halfEdge; y += 10)
		{
			vertices.push_back(sm::vec2(-halfEdge, y));
			vertices.push_back(sm::vec2(halfEdge, y));
		}
		s2::RVG::SetColor(s2::Color(0.9f, 0.7f, 0.7f));
		s2::RVG::LineWidth(1);
		s2::RVG::Lines(vertices);
		s2::RVG::LineWidth(2);
	}
}

void StageCanvas::DrawJointVisitor::Visit(Joint* joint, bool& next)
{
	joint->Draw(Joint::e_default);
	next = true;
}

}