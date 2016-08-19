#include "Particle.h"

#include <ee/SpriteRenderer.h>
#include <ee/SymbolMgr.h>

#include <easyparticle3d.h>

#include <sprite2/NodeRenderer.h>

namespace ep3dinv
{

Particle::Particle()
	: m_sym(NULL)
	, m_during(0)
{
}

Particle::~Particle()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}
}

void Particle::Update(float dt, float gravity)
{
	if (m_during > m_life) {
		return;
	}

	// up position
	for (int i = 0; i < 3; ++i) {
		m_position[i] -= m_speed[i] * dt;
	}

	// up angle
	if (m_position[2] > 0.1f) {
		m_angle -= m_rotate * dt;
	}

	// up acc
	m_speed[2] += gravity * dt;
	float velocity = sqrt(m_speed[0] * m_speed[0] + m_speed[1] * m_speed[1] + m_speed[2] * m_speed[2]);
	for (int i = 0; i < 3; ++i) {
		m_speed[i] -= m_linear_acc * m_speed[i] / velocity;
	}
	
	m_during += dt;
}

void Particle::Draw(const sm::mat4& mt) const
{
// 	if (m_during > m_life) {
// 		return;
// 	}

	sm::mat4 _mt(mt);
	_mt.Translate(0, 0);

	float scale = m_during / m_life * (m_start_scale - m_end_scale) + m_end_scale;

	sm::vec2 pos = eparticle3d::TransCoords3To2(m_position);
	s2::NodeRenderer::Draw(m_sym, _mt, pos, m_angle, 
		scale, scale/*, 0, 0, mul*/);
}

void Particle::LoadFromFile(const Json::Value& val)
{
	m_start_time = val["dead time"].asInt();
	m_life_old = m_life = val["life"].asDouble();

	for (int i = 0; i < 2; ++i) {
		m_direction_old[i] = m_direction[i] = val["direction"][i].asDouble();
	}
	for (int i = 0; i < 3; ++i) {
		m_position_old[i] = m_position[i] = val["position"][i].asDouble();
	}
	for (int i = 0; i < 3; ++i) {
		m_speed_old[i] = m_speed[i] = val["speed"][i].asDouble();
	}
	m_linear_acc = val["linear_acc"].asDouble();
	m_rotate = val["rotate"].asDouble();
	m_angle_old = m_angle = val["angle"].asDouble();

	m_sym = ee::SymbolMgr::Instance()->FetchSymbol(val["symbol"].asString());
	m_start_scale = val["start_scale"].asDouble();
	m_end_scale = val["end_scale"].asDouble();
}

void Particle::Reset()
{
	m_during = 0;
	m_life = m_life_old;
	for (int i = 0; i < 2; ++i) {
		m_direction[i] = m_direction_old[i];
	}
	for (int i = 0; i < 3; ++i) {
		m_position[i] = m_position_old[i];
	}
	for (int i = 0; i < 3; ++i) {
		m_speed[i] = m_speed_old[i];
	}
	m_angle = m_angle_old;
}

}