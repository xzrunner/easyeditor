#include "ParticleSystem.h"

namespace ep3dinv
{

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::~ParticleSystem()
{
	Clear();
}

void ParticleSystem::Update(float dt)
{

}

void ParticleSystem::Draw(const d2d::Matrix& mt) const
{

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

	int i = 0;
	Json::Value val = value["sprite"][i++];
	while (!val.isNull()) {
		Particle* p = new Particle;
		p->LoadFromFile(val);
		m_particles.push_back(p);

		val = value["sprite"][i++];
	}
}

void ParticleSystem::Clear()
{
	for_each(m_particles.begin(), m_particles.end(), DeletePointerFunctor<Particle>());
	m_particles.clear();
}

}