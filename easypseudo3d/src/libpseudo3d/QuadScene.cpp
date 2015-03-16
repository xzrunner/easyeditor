#include "QuadScene.h"
#include "StageCanvas.h"

#include <easy3d.h>

namespace epseudo3d
{

QuadScene::QuadScene(StageCanvas* canvas)
	: m_canvas(canvas)
	, m_img(NULL)
{
}

QuadScene::~QuadScene()
{
	delete m_img;
}

void QuadScene::Load()
{
	m_img = d2d::ImageMgr::Instance()->getItem("box/crate.jpg");
}

void QuadScene::Draw() const
{
	e3d::DrawCube(vec3(-5, -5, -5), vec3(5, 5, 5), m_img->textureID());

//	e3d::DrawCube(vec3(-5, -5, -5), vec3(5, 5, 5), d2d::WHITE);
}

void QuadScene::DebugDraw() const
{
	wxPaintDC dc((wxWindow*)m_canvas);

	dc.SetPen(*wxWHITE_PEN);
	dc.SetBrush(*wxWHITE_BRUSH);
	{
		ivec2 v = m_canvas->TransPos3ProjectToScreen(vec3(5, 5, 5));
		dc.DrawRectangle(v.x, v.y, 10, 10);
	}
	{
		ivec2 v = m_canvas->TransPos3ProjectToScreen(vec3(5, 5, -5));
		dc.DrawRectangle(v.x, v.y, 10, 10);
	}
	{
		ivec2 v = m_canvas->TransPos3ProjectToScreen(vec3(5, -5, 5));
		dc.DrawRectangle(v.x, v.y, 10, 10);
	}
	{
		ivec2 v = m_canvas->TransPos3ProjectToScreen(vec3(5, -5, -5));
		dc.DrawRectangle(v.x, v.y, 10, 10);
	}
	{
		ivec2 v = m_canvas->TransPos3ProjectToScreen(vec3(-5, 5, 5));
		dc.DrawRectangle(v.x, v.y, 10, 10);
	}
	{
		ivec2 v = m_canvas->TransPos3ProjectToScreen(vec3(-5, 5, -5));
		dc.DrawRectangle(v.x, v.y, 10, 10);
	}
	{
		ivec2 v = m_canvas->TransPos3ProjectToScreen(vec3(-5, -5, 5));
		dc.DrawRectangle(v.x, v.y, 10, 10);
	}
	{
		ivec2 v = m_canvas->TransPos3ProjectToScreen(vec3(-5, -5, -5));
		dc.DrawRectangle(v.x, v.y, 10, 10);
	}
}

}