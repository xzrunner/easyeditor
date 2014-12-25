#include "ViewPlane.h"

#include "samplers/Regular.h"
#include "samplers/MultiJittered.h"
#include "samplers/Jittered.h"

#include <stdio.h>

namespace rt
{

ViewPlane::ViewPlane()
	: m_hori_res(400)
	, m_vert_res(400)
	, m_pixel_size(1)
	, m_num_samples(1)
	, m_max_depth(0)
	, m_sampler(NULL)
	, m_show_out_of_gamut(false)
	, m_gamma(1)
	, m_inv_gamma(1)
{
}

ViewPlane::ViewPlane(const ViewPlane& vp)
	: m_hori_res(vp.m_hori_res)
	, m_vert_res(vp.m_vert_res)
	, m_pixel_size(vp.m_pixel_size)
	, m_num_samples(vp.m_num_samples)
	, m_max_depth(vp.m_max_depth)
	, m_sampler(NULL)
	, m_show_out_of_gamut(vp.m_show_out_of_gamut)
	, m_gamma(vp.m_gamma)
	, m_inv_gamma(vp.m_inv_gamma)
{
	SetSampler(vp.m_sampler);
}

ViewPlane::~ViewPlane()
{
	if (m_sampler) {
		m_sampler->Release();
	}
}

void ViewPlane::SetSampler(Sampler* sampler)
{
	obj_assign((const Object*&)m_sampler, sampler);
	m_num_samples = sampler->GetNumSamples();
}

void ViewPlane::SetSamples(int n)
{
	m_num_samples = n;

	if (m_sampler) {
		delete m_sampler;
		m_sampler = NULL;
	}

  	if (m_num_samples > 1) {
  		m_sampler = new MultiJittered(m_num_samples);
  	} else {
  		m_sampler = new Regular(1);
  	}
}

}