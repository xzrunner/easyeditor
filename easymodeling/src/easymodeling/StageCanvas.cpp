#include "StageCanvas.h"
#include "StagePanel.h"
#include "DrawUtils.h"

#include <ee/EditPanelImpl.h>
#include <ee/FetchAllVisitor.h>
#include <ee/SpriteRenderer.h>
#include <ee/Sprite.h>
#include <ee/PrimitiveDraw.h>

namespace emodeling
{

StageCanvas::StageCanvas(StagePanel* editPanel)
	: ee::OrthoCanvas(editPanel, editPanel->GetStageImpl())
	, m_stage_panel(editPanel)
{
	m_bg_color.Set(0, 0, 0, 1);
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
	std::vector<ee::Sprite*> sprites;
	m_stage_panel->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		ee::SpriteRenderer::Instance()->Draw(sprites[i]);
		DrawUtils::DrawBody(static_cast<Body*>(sprites[i]->GetUserData()), DrawUtils::e_default);
	}
}

void StageCanvas::DrawCrossLine() const
{
	const float halfEdge = 1000.0f;
	std::vector<ee::Vector> vertices(4);
	vertices[0].x = -halfEdge;
	vertices[1].x = halfEdge;
	vertices[2].y = -halfEdge;
	vertices[3].y = halfEdge;
	ee::PrimitiveDraw::DrawLines(vertices, ee::Colorf(0.7f, 0.9f, 0.7f), 1);
}

void StageCanvas::DrawLines() const
{
 	const float halfEdge = 100.0f;
	// green
	{
		std::vector<ee::Vector> vertices;
		size_t size = halfEdge * 2 * 2 * 2;
		vertices.reserve(size);
		for (float x = -halfEdge; x <= halfEdge; x += 1.0f)
		{
			vertices.push_back(ee::Vector(x, -halfEdge));
			vertices.push_back(ee::Vector(x, halfEdge));
		}
		for (float y = -halfEdge; y <= halfEdge; y += 1.0f)
		{
			vertices.push_back(ee::Vector(-halfEdge, y));
			vertices.push_back(ee::Vector(halfEdge, y));
		}
		ee::PrimitiveDraw::DrawLines(vertices, ee::Colorf(0.7f, 0.9f, 0.7f), 1);
	}
	// red
	{
		std::vector<ee::Vector> vertices;
		size_t size = halfEdge * 0.1f * 2 * 2 * 2;
		vertices.reserve(size);
		for (float x = -halfEdge; x <= halfEdge; x += 10)
		{
			vertices.push_back(ee::Vector(x, -halfEdge));
			vertices.push_back(ee::Vector(x, halfEdge));
		}
		for (float y = -halfEdge; y <= halfEdge; y += 10)
		{
			vertices.push_back(ee::Vector(-halfEdge, y));
			vertices.push_back(ee::Vector(halfEdge, y));
		}
		ee::PrimitiveDraw::DrawLines(vertices, ee::Colorf(0.9f, 0.7f, 0.7f), 1);
	}
}

void StageCanvas::DrawJointVisitor::Visit(ee::Object* object, bool& next)
{
	Joint* joint = static_cast<Joint*>(object);
	joint->Draw(Joint::e_default);
	next = true;
}

}