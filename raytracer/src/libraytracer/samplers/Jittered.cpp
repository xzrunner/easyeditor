#include "Jittered.h"

#include "maths/maths.h"

#include <math.h>

namespace rt
{

Jittered::Jittered(int num)
	: Sampler(num)
{
	m_num_sets = 1;
	GenerateSamples();
}

void Jittered::GenerateSamples()
{
	int n = (int)sqrt((float)m_num_samples);

	for (int p = 0; p < m_num_samples; p++) {
		for (int j = 0; j < n; j++) {
			for (int k = 0; k < n; k++) {
				Point2D sp((k + rand_float()) / n, (j + rand_float()) / n);
				m_samples.push_back(sp);
			}
		}
	}
}

}