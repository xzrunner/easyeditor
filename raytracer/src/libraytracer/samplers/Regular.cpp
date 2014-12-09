#include "Regular.h"

#include <math.h>

namespace rt
{

Regular::Regular(int num)
	: Sampler(num)
{
	GenerateSamples();
}

void Regular::GenerateSamples()
{
	int n = (int) sqrt((float)m_num_samples);

	for (int j = 0; j < m_num_sets; j++) {
		for (int p = 0; p < n; p++) {
			for (int q = 0; q < n; q++) {
				m_samples.push_back(Point2D((q + 0.5f) / n, (p + 0.5f) / n));
			}
		}
	}
}

}