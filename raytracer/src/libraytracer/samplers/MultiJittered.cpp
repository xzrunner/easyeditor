#include "MultiJittered.h"

#include "maths/maths.h"

#include <math.h>

namespace rt
{

MultiJittered::MultiJittered(int num)
	: Sampler(num)
{
	GenerateSamples();
}

void MultiJittered::GenerateSamples()
{
	// m_num_samples needs to be a perfect square

	int n = (int)sqrt((float)m_num_samples);
	float subcell_width = 1.0f / ((float) m_num_samples);

	// fill the m_samples array with dummy points to allow us to use the [ ] notation when we set the
	// initial patterns

	Point2D fill_point;
	for (int j = 0; j < m_num_samples * m_num_sets; j++) {
		m_samples.push_back(fill_point);
	}

	// distribute points in the initial patterns

	for (int p = 0; p < m_num_sets; p++) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				m_samples[i * n + j + p * m_num_samples].x = (i * n + j) * subcell_width + rand_float(0, subcell_width);
				m_samples[i * n + j + p * m_num_samples].y = (j * n + i) * subcell_width + rand_float(0, subcell_width);
			}
		}
	}

	// shuffle x coordinates

	for (int p = 0; p < m_num_sets; p++) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				int k = rand_int(j, n - 1);
				float t = m_samples[i * n + j + p * m_num_samples].x;
				m_samples[i * n + j + p * m_num_samples].x = m_samples[i * n + k + p * m_num_samples].x;
				m_samples[i * n + k + p * m_num_samples].x = t;
			}
		}
	}

	// shuffle y coordinates

	for (int p = 0; p < m_num_sets; p++) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				int k = rand_int(j, n - 1);
				float t = m_samples[j * n + i + p * m_num_samples].y;
				m_samples[j * n + i + p * m_num_samples].y = m_samples[k * n + i + p * m_num_samples].y;
				m_samples[k * n + i + p * m_num_samples].y = t;
			}
		}
	}
}

}