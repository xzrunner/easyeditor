#include "Sampler.h"

#include "maths/maths.h"
#include "utilities/Constants.h"

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

// ------------------------------------------------------------------- map_samples_to_unit_disk
// Maps the 2D sample points in the square [-1,1] X [-1,1] to a unit disk, using Peter Shirley's
// concentric map function
// explained on page 122
void Sampler::MapSamplesToUnitDisk()
{
	int size = m_samples.size();
	float r, phi;		// polar coordinates
	Point2D sp; 		// sample point on unit disk

	m_disk_samples.reserve(size);

	for (int j = 0; j < size; j++) {
		// map sample point to [-1, 1] X [-1,1]

		sp.x = 2.0 * m_samples[j].x - 1.0;
		sp.y = 2.0 * m_samples[j].y - 1.0;

		if (sp.x > -sp.y) {			// sectors 1 and 2
			if (sp.x > sp.y) {		// sector 1
				r = sp.x;
				phi = sp.y / sp.x;
			}
			else {					// sector 2
				r = sp.y;
				phi = 2.0 - sp.x / sp.y;
			}
		}
		else {						// sectors 3 and 4
			if (sp.x < sp.y) {		// sector 3
				r = -sp.x;
				phi = 4.0 + sp.y / sp.x;
			}
			else {					// sector 4
				r = -sp.y;
				if (sp.y != 0.0)	// avoid division by zero at origin
					phi = 6.0 - sp.x / sp.y;
				else
					phi  = 0.0;
			}
		}

		phi *= PI / 4.0;

		float px = r * cos(phi);
		float py = r * sin(phi);

		m_disk_samples.push_back(Point2D(px, py));
	}

	m_samples.clear();
}

// ------------------------------------------------------------------- map_samples_to_hemisphere
// Maps the 2D sample points to 3D points on a unit hemisphere with a cosine power
// density distribution in the polar angle
// explained on page 129
void Sampler::MapSamplesToHemisphere(const float exp)
{
	int size = m_samples.size();
	m_hemisphere_samples.reserve(m_num_samples * m_num_sets);

	for (int j = 0; j < size; j++) {
		float cos_phi = (float)cos(2.0 * PI * m_samples[j].x);
		float sin_phi = (float)sin(2.0 * PI * m_samples[j].x);	
		float cos_theta = pow(1.0f - m_samples[j].y, 1.0f / (exp + 1.0f));
		float sin_theta = sqrt (1.0f - cos_theta * cos_theta);
		float pu = sin_theta * cos_phi;
		float pv = sin_theta * sin_phi;
		float pw = cos_theta;
		m_hemisphere_samples.push_back(Point3D(pu, pv, pw)); 
	}
}

const Point2D& Sampler::SampleUnitSquare() const
{
	// start of a new pixel
	if (m_count % m_num_samples == 0) {
		// random index m_jump initialised to zero in constructor
		m_jump = (rand_int() % m_num_sets) * m_num_samples;
	}

	return m_samples[m_jump + m_shuffled_indices[m_jump + m_count++ % m_num_samples]];
}

const Point2D& Sampler::SampleUnitDisk() const
{
	// start of a new pixel
	if (m_count % m_num_samples == 0) {
		m_jump = (rand_int() % m_num_sets) * m_num_samples;
	}

	return (m_disk_samples[m_jump + m_shuffled_indices[m_jump + m_count++ % m_num_samples]]);
}

const Point3D& Sampler::SampleHemisphere()
{
	// start of a new pixel
	if (m_count % m_num_samples == 0) {
		m_jump = (rand_int() % m_num_sets) * m_num_samples;
	}

	return (m_hemisphere_samples[m_jump + m_shuffled_indices[m_jump + m_count++ % m_num_samples]]);
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