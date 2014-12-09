#ifndef _RT_VIEW_PLANE_H_
#define _RT_VIEW_PLANE_H_

namespace rt
{

class Sampler;

class ViewPlane
{
public:
	ViewPlane();
	~ViewPlane();

	int GetWidth() const { return m_hori_res; }
	int GetHeight() const { return m_vert_res; }
	void SetWidth(int w) { m_hori_res = w; }
	void SetHeight(int h) { m_vert_res = h; }

	float GetPixelSize() const { return m_pixel_size; }
	void SetPixelSize(float sz) { m_pixel_size = sz; }

	int GetSamplesNum() const { return m_num_samples; }
	const Sampler* GetSampler() const { return m_sampler; }
	void SetSamples(int n);

	bool ShowOutOfGamut() const { return m_show_out_of_gamut; }

	float GetGamma() const { return m_gamma; }
	float GetInvGamma() const { return m_inv_gamma; }

private:
	// resolution
	int m_hori_res, m_vert_res;

	// pixel size
	float m_pixel_size;							

	// number of samples per pixel
	int	m_num_samples;

	Sampler* m_sampler;

	// display red if RGBColor out of gamut
	bool m_show_out_of_gamut;

	// gamma correction factor
	float m_gamma;
	// the inverse of the gamma correction factor
	float m_inv_gamma;

}; // ViewPlane

}

#endif // _RT_VIEW_PLANE_H_