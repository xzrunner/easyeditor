#include "StageCanvas.h"
#include "StagePanel.h"
#include "DrawUtils.h"

namespace emodeling
{

StageCanvas::StageCanvas(StagePanel* editPanel)
	: d2d::OrthoCanvas(editPanel)
{
	m_bg_color.set(0, 0, 0, 1);
}

StageCanvas::~StageCanvas()
{
}

void StageCanvas::OnDrawSprites() const
{
	DrawGuideLines();

	DrawSprites();
	static_cast<StagePanel*>(m_stage)->traverseJoints(DrawJointVisitor());
	m_stage->DrawEditOP();
}

void StageCanvas::DrawGuideLines() const
{
	DrawCrossLine();
}

void StageCanvas::DrawSprites() const
{
	StagePanel* editPanel = static_cast<StagePanel*>(m_stage);
	std::vector<d2d::ISprite*> sprites;
	editPanel->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::SpriteRenderer::Instance()->Draw(sprites[i]);
		DrawUtils::drawBody(static_cast<libmodeling::Body*>(sprites[i]->GetUserData()), DrawUtils::e_default);
	}
}

void StageCanvas::DrawCrossLine() const
{
	const float halfEdge = 1000.0f;
	std::vector<d2d::Vector> vertices(4);
	vertices[0].x = -halfEdge;
	vertices[1].x = halfEdge;
	vertices[2].y = -halfEdge;
	vertices[3].y = halfEdge;
	d2d::PrimitiveDraw::drawLines(vertices, d2d::Colorf(0.7f, 0.9f, 0.7f), 1);
}

void StageCanvas::DrawLines() const
{
 	const float halfEdge = 100.0f;
	// green
	{
		std::vector<d2d::Vector> vertices;
		size_t size = halfEdge * 2 * 2 * 2;
		vertices.reserve(size);
		for (float x = -halfEdge; x <= halfEdge; x += 1.0f)
		{
			vertices.push_back(d2d::Vector(x, -halfEdge));
			vertices.push_back(d2d::Vector(x, halfEdge));
		}
		for (float y = -halfEdge; y <= halfEdge; y += 1.0f)
		{
			vertices.push_back(d2d::Vector(-halfEdge, y));
			vertices.push_back(d2d::Vector(halfEdge, y));
		}
		d2d::PrimitiveDraw::drawLines(vertices, d2d::Colorf(0.7f, 0.9f, 0.7f), 1);
	}
	// red
	{
		std::vector<d2d::Vector> vertices;
		size_t size = halfEdge * 0.1f * 2 * 2 * 2;
		vertices.reserve(size);
		for (float x = -halfEdge; x <= halfEdge; x += 10)
		{
			vertices.push_back(d2d::Vector(x, -halfEdge));
			vertices.push_back(d2d::Vector(x, halfEdge));
		}
		for (float y = -halfEdge; y <= halfEdge; y += 10)
		{
			vertices.push_back(d2d::Vector(-halfEdge, y));
			vertices.push_back(d2d::Vector(halfEdge, y));
		}
		d2d::PrimitiveDraw::drawLines(vertices, d2d::Colorf(0.9f, 0.7f, 0.7f), 1);
	}
}

void StageCanvas::DrawJointVisitor::Visit(d2d::Object* object, bool& bFetchNext)
{
	libmodeling::Joint* joint = static_cast<libmodeling::Joint*>(object);
	joint->draw(libmodeling::Joint::e_default);
	bFetchNext = true;
}

}