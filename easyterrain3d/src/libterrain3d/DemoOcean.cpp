#include "DemoOcean.h"

#include <ee/Image.h>

#include <easy3d/Camera.h>

#include <glp_loop.h>
#include <SM_Calc.h>
#include <node3/AABB.h>
#include <node3/PrimitiveDraw.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Model3Shader.h>

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
	m_cam.SetPosition(sm::vec3(0, -8, 8));
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
	static uint32_t last = 0;
	if (last == 0) {
		last = glp_get_time();
		return;
	}

	uint32_t curr = glp_get_time();
	float dt = (curr - last) * 0.000001f;
	last = curr;

	Update(dt);

	n3::AABB aabb;
	aabb.Combine(sm::vec3(-5, -5, -5));
	aabb.Combine(sm::vec3(5, 5, 5));
	n3::PrimitiveDraw::SetColor(0xffffffff);
	n3::PrimitiveDraw::Cube(aabb);

	sm::vec2 uv_left_low;
	uv_left_low.x = m_uv_base.x - 1;
	uv_left_low.y = m_uv_base.y - 1;

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::MODEL3);
	sl::Model3Shader* shader = static_cast<sl::Model3Shader*>(mgr->GetShader());

	sl::Model3Shader::Material material;
	material.ambient.Assign(0.04f, 0.04f, 0.04f);
	material.diffuse.Assign(1, 1, 1);
	material.specular.Assign(1, 1, 1);
	material.shininess = 50;
	material.tex_id = m_image->GetTexID();
	shader->SetMaterial(material);

	std::vector<uint16_t> indices = { 0, 1, 2 };

	for (int y = 0; y < ROW; ++y) {
		for (int x = 0; x < COL; ++x) {
			float vx_min = EDGE * x / COL - 0.5f*EDGE, vx_max = EDGE * (x+1) / COL - 0.5f*EDGE,
				  vy_min = EDGE * y / COL - 0.5f*EDGE, vy_max = EDGE * (y+1) / COL - 0.5f*EDGE;
			float h_minmin = GetHeight(vx_min, vy_min), h_minmax = GetHeight(vx_min, vy_max),
				  h_maxmin = GetHeight(vx_max, vy_min), h_maxmax = GetHeight(vx_max, vy_max);
			float tx_min = (float)x / COL, tx_max = (float)(x+1) / COL,
				  ty_min = (float)y / COL, ty_max = (float)(y+1) / COL;
			{
				std::vector<sm::vec2> texcoords = {
					sm::vec2(tx_min, ty_min),
					sm::vec2(tx_min, ty_max),
					sm::vec2(tx_max, ty_max)
				};
				sm::vec2 center = (texcoords[0] + texcoords[1] + texcoords[2]) / 3;
				sm::vec2 base;
				int ix = center.x - uv_left_low.x,
					iy = center.y - uv_left_low.y;
				base.x = uv_left_low.x + ix;
				base.y = uv_left_low.y + iy;
				for (int i = 0; i < 3; ++i) {
					texcoords[i] = texcoords[i] - base;
				}

				std::vector<float> vertices = {
					vx_min, vy_min, h_minmin, texcoords[0].x, texcoords[0].y,
					vx_min, vy_max, h_minmax, texcoords[1].x, texcoords[1].y,
					vx_max, vy_max, h_maxmax, texcoords[2].x, texcoords[2].y,
				};
				shader->Draw(&vertices[0], vertices.size(), &indices[0], indices.size(), false, true);
			}
			{
				std::vector<sm::vec2> texcoords = {
					sm::vec2(tx_max, ty_max),
					sm::vec2(tx_max, ty_min),
					sm::vec2(tx_min, ty_min)
				};

				sm::vec2 center = (texcoords[0] + texcoords[1] + texcoords[2]) / 3;
				sm::vec2 base;
				int ix = center.x - uv_left_low.x,
					iy = center.y - uv_left_low.y;
				base.x = uv_left_low.x + ix;
				base.y = uv_left_low.y + iy;
				for (int i = 0; i < 3; ++i) {
					texcoords[i] = texcoords[i] - base;
				}

				std::vector<float> vertices = {
					vx_max, vy_max, h_maxmax, texcoords[0].x, texcoords[0].y,
					vx_max, vy_min, h_maxmin, texcoords[1].x, texcoords[1].y,
					vx_min, vy_min, h_minmin, texcoords[2].x, texcoords[2].y,
				};
				shader->Draw(&vertices[0], vertices.size(), &indices[0], indices.size(), false, true);
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
		float dis = sm::dis_pos3_to_pos3(sm::vec3(x, y, 0), sm::vec3(w.pos.x, w.pos.y, 0));
		h += w.len * cos(dis);
	}
	return h;
}

}