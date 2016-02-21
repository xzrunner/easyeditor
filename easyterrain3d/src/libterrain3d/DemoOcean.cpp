#include "DemoOcean.h"

#include <ee/Image.h>

namespace eterrain3d
{

static const float EDGE = 10;

static const int ROW = 20;
static const int COL = 20;

DemoOcean::DemoOcean(e3d::Camera& cam)
	: m_cam(cam)
	, m_image(NULL)
{
	m_uv_spd.x = 0.02f;
	m_uv_spd.y = 0.01f;
}

void DemoOcean::Load()
{
	m_cam.SetPosition(ee::vec3(0, -8, 8));
	m_cam.Rotate(0, 40);

	m_image = ee::ImageMgr::Instance()->GetItem("ocean/water2_256.png");

	Wave w;
	w.len = 0.2f;
	w.speed.x = 0.5f;
	w.speed.y = 0.5f;
	m_waves.push_back(w);

	w.len = 0.1f;
	w.speed.x = -0.2f;
	w.speed.y = 0.5f;
	m_waves.push_back(w);
}

void DemoOcean::Draw() const
{
	static clock_t last = 0;
	if (last == 0) {
		last = clock();
		return;
	}

	clock_t curr = clock();
	float dt = (float)(curr - last) / CLOCKS_PER_SEC;
	last = curr;	
	Update(dt);

	e3d::AABB aabb;
	aabb.Combine(ee::vec3(-5, -5, -5));
	aabb.Combine(ee::vec3(5, 5, 5));
	e3d::DrawCube(aabb, ee::WHITE);

	ee::vec2 uv_left_low;
	uv_left_low.x = m_uv_base.x - 1;
	uv_left_low.y = m_uv_base.y - 1;

 	e3d::ShaderMgr* shader = e3d::ShaderMgr::Instance();
 	shader->Sprite();
	for (int y = 0; y < ROW; ++y) {
		for (int x = 0; x < COL; ++x) {
			float vx_min = EDGE * x / COL - 0.5f*EDGE, vx_max = EDGE * (x+1) / COL - 0.5f*EDGE,
				  vy_min = EDGE * y / COL - 0.5f*EDGE, vy_max = EDGE * (y+1) / COL - 0.5f*EDGE;
			float h_minmin = GetHeight(vx_min, vy_min), h_minmax = GetHeight(vx_min, vy_max),
				  h_maxmin = GetHeight(vx_max, vy_min), h_maxmax = GetHeight(vx_max, vy_max);
			float tx_min = (float)x / COL, tx_max = (float)(x+1) / COL,
				  ty_min = (float)y / COL, ty_max = (float)(y+1) / COL;
			{
				ee::vec3 vertices[] = {ee::vec3(vx_min, vy_min, h_minmin), ee::vec3(vx_min, vy_max, h_minmax), ee::vec3(vx_max, vy_max, h_maxmax)};
				ee::vec2 texcoords[] = {ee::vec2(tx_min, ty_min), ee::vec2(tx_min, ty_max), ee::vec2(tx_max, ty_max)};

				ee::vec2 center = (texcoords[0] + texcoords[1] + texcoords[2]) / 3;
				ee::vec2 base;
				int ix = center.x - uv_left_low.x,
					iy = center.y - uv_left_low.y;
				base.x = uv_left_low.x + ix;
				base.y = uv_left_low.y + iy;
				for (int i = 0; i < 3; ++i) {
					texcoords[i] = texcoords[i] - base;
				}

				shader->DrawTri(vertices, texcoords, m_image->GetTexID());
			}
			{
				ee::vec3 vertices[] = {ee::vec3(vx_max, vy_max, h_maxmax), ee::vec3(vx_max, vy_min, h_maxmin), ee::vec3(vx_min, vy_min, h_minmin)};
				ee::vec2 texcoords[] = {ee::vec2(tx_max, ty_max), ee::vec2(tx_max, ty_min), ee::vec2(tx_min, ty_min)};

				ee::vec2 center = (texcoords[0] + texcoords[1] + texcoords[2]) / 3;
				ee::vec2 base;
				int ix = center.x - uv_left_low.x,
					iy = center.y - uv_left_low.y;
				base.x = uv_left_low.x + ix;
				base.y = uv_left_low.y + iy;
				for (int i = 0; i < 3; ++i) {
					texcoords[i] = texcoords[i] - base;
				}

				shader->DrawTri(vertices, texcoords, m_image->GetTexID());
			}
		}
	}	
}

void DemoOcean::Update(float dt) const
{
	for (int i = 0, n = m_waves.size(); i < n; ++i) {
		Wave& w = m_waves[i];
		w.pos = w.pos + w.speed * dt;
	}

	m_uv_base = m_uv_base + m_uv_spd * dt;
	if (m_uv_base.x >= 1) m_uv_base.x -= 1;
	if (m_uv_base.x < 0) m_uv_base.x += 1;
	if (m_uv_base.y >= 1) m_uv_base.y -= 1;
	if (m_uv_base.y < 0) m_uv_base.y += 1;
}

float DemoOcean::GetHeight(float x, float y) const
{
	float h = -5;
	for (int i = 0, n = m_waves.size(); i < n; ++i) {
		const Wave& w = m_waves[i];
		float dis = e3d::Math3::GetDistance(ee::vec3(x, y, 0), ee::vec3(w.pos.x, w.pos.y, 0));
		h += w.len * cos(dis);
	}
	return h;
}

}