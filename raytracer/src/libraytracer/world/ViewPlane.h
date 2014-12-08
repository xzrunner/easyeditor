#ifndef _RT_VIEW_PLANE_H_
#define _RT_VIEW_PLANE_H_

namespace rt
{

class Sampler;

class ViewPlane
{
public:
	ViewPlane();

	int GetWidth() const { return m_hori_res; }
	int GetHeight() const { return m_vert_res; }
	void SetWidth(int w) { m_hori_res = w; }
	void SetHeight(int h) { m_vert_res = h; }

	float GetPixelSize() const { return m_pixel_size; }
	void SetPixelSize(float sz) { m_pixel_size = sz; }

	int GetSamplesNum() const { return m_num_samples; }
	void SetSamplesNum(int n) { m_num_samples = n; }
	
	const Sampler* GetSampler() const { return m_sampler; }

private:
	// resolution
	int m_hori_res, m_vert_res;

	// pixel size
	float m_pixel_size;							

	// number of samples per pixel
	int	m_num_samples;

	Sampler* m_sampler;

}; // ViewPlane

}

#endif // _RT_VIEW_PLANE_H_