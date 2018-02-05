#pragma once

#include <SM_Vector.h>
#include <SM_Matrix.h>

namespace pt3
{

class Camera
{
public:
	Camera();
	Camera(const sm::vec3& pos, const sm::vec3& target, const sm::vec3& up);

	void Slide(float du, float dv, float dn);

	// rotate
	void Roll(float angle);
	void Yaw(float angle);
	void Pitch(float angle);
	void SetUpDir(const sm::vec3& up);

	// translate
	void Translate(float dx, float dy);
	void MoveToward(float offset);
	void Move(const sm::vec3& dir, float offset);
	void AimAtTarget();

	const sm::vec3& GetPos() const { return m_pos; }
	const sm::vec3& GetTarget() const { return m_target; }
	float GetDistance() const { return m_distance; }
	const sm::vec3& GetUpDir() const { return m_v; }

	sm::mat4 GetModelViewMat() const;
	sm::mat4 GetProjectionMat() const;
	sm::mat4 GetRotateMat() const;

	float GetNear() const { return m_znear; }
	float GetFar() const { return m_zfar; }
	float GetAspect() const { return m_aspect; }
	float GetAngleOfView() const { return m_angle_of_view; }

	const sm::vec3 GetToward() const { return m_n; }
	
	void SetAspect(float aspect) { m_aspect = aspect; }

	void Reset();
	void Reset(const sm::vec3& pos, const sm::vec3& target, const sm::vec3& up);

private:
	void InitUVN(const sm::vec3& up);

private:
	// pos
	sm::vec3 m_pos, m_target;
	float m_distance;

	// angle
	sm::vec3 m_u, m_v, m_n;

	// store for reset
	sm::vec3 m_init_pos;
	sm::vec3 m_init_target;
	sm::vec3 m_init_up;

	// projection
	float m_znear, m_zfar;
	float m_aspect;
	float m_angle_of_view;

}; // Camera

}