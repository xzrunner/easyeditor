#ifndef _EASYP3DINV_PARTICLE_H_
#define _EASYP3DINV_PARTICLE_H_

namespace ep3dinv
{

class Particle
{
public:
	Particle();
	~Particle();

	void LoadFromFile(const Json::Value& val);

private:
	float m_start_time;
	float m_life;

	float m_direction[2];
	float m_position[3];
	float m_speed[3];
	float m_linear_acc;
	float m_rotate;
	float m_angle;

	d2d::ISymbol* m_symbol;
	float m_start_scale, m_end_scale; 

}; // Particle

}

#endif // _EASYP3DINV_PARTICLE_H_