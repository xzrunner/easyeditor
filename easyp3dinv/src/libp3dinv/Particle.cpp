#include "Particle.h"

#include <easyparticle3d.h>

namespace ep3dinv
{

Particle::Particle()
	: m_symbol(NULL)
	, m_during(0)
{
}

Particle::~Particle()
{
	if (m_symbol) {
		m_symbol->Release();
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

void Particle::Draw(const d2d::Matrix& mt) const
{
	if (m_during > m_life) {
		return;
	}

	d2d::Matrix _mt(mt);
	_mt.translate(0, 0);

	float scale = m_during / m_life * (m_start_scale - m_end_scale) + m_end_scale;

	d2d::Vector pos = eparticle3d::TransCoords3To2(m_position);
	d2d::SpriteRenderer::Instance()->Draw(m_symbol, _mt, pos, m_angle, 
		scale, scale/*, 0, 0, multi*/);
}

void Particle::LoadFromFile(const Json::Value& val)
{
	m_start_time = val["dead time"].asInt();
	m_life = val["life"].asDouble();

	for (int i = 0; i < 2; ++i) {
		m_direction[i] = val["direction"][i].asDouble();
	}
	for (int i = 0; i < 3; ++i) {
		m_position[i] = val["position"][i].asDouble();
	}
	for (int i = 0; i < 3; ++i) {
		m_speed[i] = val["speed"][i].asDouble();
	}
	m_linear_acc = val["linear_acc"].asDouble();
	m_rotate = val["rotate"].asDouble();
	m_angle = val["angle"].asDouble();
}

}