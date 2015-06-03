#ifndef _EASYSHADOW_SHADOW_H_
#define _EASYSHADOW_SHADOW_H_

#include <drag2d.h>

namespace eshadow
{

class Shadow
{
public:
	Shadow(float radius);
	~Shadow();

	void Draw(const d2d::Matrix& mt) const;

	void Build();

private:
	void BuildOuterLoop();

private:
	float m_radius;

	std::vector<d2d::Vector> m_inner_loop;
	std::vector<d2d::Vector> m_outer_loop;

	d2d::Colorf m_inner_color, m_outer_color;

	std::vector<d2d::Vector> m_tris;	
	std::vector<d2d::Colorf> m_colors;	

}; // Shadow

}

#endif // _EASYSHADOW_SHADOW_H_