#include "Sampler.h"

#include "maths/maths.h"

#include <algorithm>

namespace rt
{

Sampler::Sampler()
	: m_num_samples(1)
	, m_num_sets(83)
	, m_count(0)
	, m_jump(0)
{
	m_samples.reserve(m_num_samples * m_num_sets);
	SetupShuffledIndices();
}

Sampler::Sampler(int num)
	: m_num_samples(num)
	, m_num_sets(83)
	, m_count(0)
	, m_jump(0)
{
	m_samples.reserve(m_num_samples * m_num_sets);
	SetupShuffledIndices();
}

const Point2D& Sampler::SampleUnitSquare() const
{
	// start of a new pixel
	if (m_count % m_num_samples == 0) {
		// random index jump initialised to zero in constructor
		m_jump = (rand_int() % m_num_sets) * m_num_samples;
	}

	return m_samples[m_jump + m_shuffled_indices[m_jump + m_count++ % m_num_samples]];
}

void Sampler::SetupShuffledIndices()
{
	m_shuffled_indices.reserve(m_num_samples * m_num_sets);

	std::vector<int> indices;
	for (int j = 0; j < m_num_samples; j++) {
		indices.push_back(j);
	}

	for (int p = 0; p < m_num_sets; p++) { 
		random_shuffle(indices.begin(), indices.end());
		for (int j = 0; j < m_num_samples; j++) {
			m_shuffled_indices.push_back(indices[j]);
		}
	}
}

}