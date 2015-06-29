#include "ParticleSystem.h"

namespace ep3dinv
{

ParticleSystem::ParticleSystem()
	: m_during(0)
{
}

ParticleSystem::~ParticleSystem()
{
	Clear();
}

void ParticleSystem::Update(float dt)
{
	m_during += dt;

	for (int i = 0, n = m_particles.size(); i < n; ++i) {
		Particle* p = m_particles[i];
		if (m_during > p->GetStartTime()) {
			p->Update(dt, m_gravity);
		}
	}
}

void ParticleSystem::Draw(const d2d::Matrix& mt) const
{
	for (int i = 0, n = m_particles.size(); i < n; ++i) {
		Particle* p = m_particles[i];
		if (m_during > p->GetStartTime()) {
			p->Draw(mt);
		}
	}	
}

void ParticleSystem::Reset()
{
	m_during = 0;
	for (int i = 0, n = m_particles.size(); i < n; ++i) {
		m_particles[i]->Reset();
	}
}

void ParticleSystem::LoadFromFile(const std::string& filepath)
{
	Clear();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	m_gravity = value["ps"]["gravity"].asDouble();

	int i = 0;
	Json::Value val = value["sprite"][i++];
	while (!val.isNull()) {
		Particle* p = new Particle;
		p->LoadFromFile(val);
		m_particles.push_back(p);

		val = value["sprite"][i++];
	}

	float final_time = 0;
	for (int i = 0, n = m_particles.size(); i < n; ++i) {
		final_time = std::max(final_time, m_particles[i]->GetStartTime());
	}
	for (int i = 0, n = m_particles.size(); i < n; ++i) {
		Particle* p = m_particles[i];
		float start_time = float(final_time - p->GetStartTime()) / CLOCKS_PER_SEC;
		p->SetStartTime(start_time);
	}
}

void ParticleSystem::Clear()
{
	for_each(m_particles.begin(), m_particles.end(), DeletePointerFunctor<Particle>());
	m_particles.clear();
}

}