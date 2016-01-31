#include "Projection3DScene.h"
#include "StageCanvas3D.h"

namespace epseudo3d
{

static const float SCALE = 0.1f;

Projection3DScene::Projection3DScene(StageCanvas3D* canvas)
	: m_canvas(canvas)
{
}

Projection3DScene::~Projection3DScene()
{
}

void Projection3DScene::Store(const char* filename) const
{

}

void Projection3DScene::Load(const char* filename)
{
	m_images.push_back(ee::ImageMgr::Instance()->GetItem("mmzb/1002_3_lv5.png"));
	m_positions.push_back(vec2(0, 0));

	m_images.push_back(ee::ImageMgr::Instance()->GetItem("mmzb/1005_3_lv12.png"));
	m_positions.push_back(vec2(0, 200));

	e3d::Camera& cam = m_canvas->GetCamera3();
	cam.SetPosition(vec3(3.7414827, 47.854599, 70.245010));
	cam.SetRotHeading(6.9000001);
	cam.SetRotElevation(-128.39999);
}

void Projection3DScene::Draw() const
{
	for (int i = 0, n = m_images.size(); i < n; ++i) {
		if (i == 0) {
			continue;
		}
		DrawSprite(m_images[i], m_positions[i].x, m_positions[i].y);
	}
}

void Projection3DScene::DebugDraw() const
{

}

void Projection3DScene::DrawSprite(ee::Image* img, int x, int y) const
{
	x *= SCALE;
	y *= SCALE;

	float w = img->GetClippedWidth() * 0.5f * SCALE,
		h = img->GetClippedHeight() * 0.5f * SCALE;
	
	std::vector<vec3> vertices;
	std::vector<vec2> texcoords;

	vertices.push_back(vec3(x-w, 0, y-w));
	texcoords.push_back(vec2(0, 1));
	vertices.push_back(vec3(x-w, 0, y+w));
	texcoords.push_back(vec2(0, 0));
	vertices.push_back(vec3(x+w, 0, y+w));
	texcoords.push_back(vec2(1, 0));

	vertices.push_back(vec3(x-w, 0, y-w));
	texcoords.push_back(vec2(0, 1));
	vertices.push_back(vec3(x+w, 0, y-w));
	texcoords.push_back(vec2(1, 1));
	vertices.push_back(vec3(x+w, 0, y+w));
	texcoords.push_back(vec2(1, 0));

	e3d::ShaderMgr* shader = e3d::ShaderMgr::Instance();
	shader->Sprite();
	shader->DrawTri(&vertices[0].x, &texcoords[0].x, 6, img->GetTexID());
}

}