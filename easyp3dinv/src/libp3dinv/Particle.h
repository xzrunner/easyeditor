#ifndef _EASYP3DINV_PARTICLE_H_
#define _EASYP3DINV_PARTICLE_H_

#include <SM_Matrix.h>

#include <json/json.h>

namespace ee { class Symbol; }

namespace ep3dinv
{

class Particle
{
public:
	Particle();
	~Particle();

	void Update(float dt, float gravity);
	void Draw(const sm::mat4& mt) const;

	void LoadFromFile(const Json::Value& val);

	float GetStartTime() const { return m_start_time; }
	void SetStartTime(float start_time) { 
		m_start_time = start_time; 
	}

	void Reset();

private:
	float m_start_time;
	float m_life, m_during;

	float m_direction[2];
	float m_position[3];
	float m_speed[3];
	float m_linear_acc;
	float m_rotate;
	float m_angle;

	ee::Symbol* m_sym;
	float m_start_scale, m_end_scale; 

private:
	// for reset, store old val

	float m_life_old;

	float m_direction_old[2];
	float m_position_old[3];
	float m_speed_old[3];
	float m_angle_old;

}; // Particle

}

#endif // _EASYP3DINV_PARTICLE_H_