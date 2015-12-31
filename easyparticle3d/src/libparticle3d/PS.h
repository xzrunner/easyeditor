#ifndef _EASYPARTICLE3D_PS_H_
#define _EASYPARTICLE3D_PS_H_

namespace eparticle3d
{

class PS
{
public:
	void UpdateTime();
	float GetTime() const { return m_time; }

	static PS* Instance();

private:
	PS();

	void Init();

private:
	float m_time;

private:
	static PS* m_instance;

}; // PS

}

#endif // _EASYPARTICLE3D_PS_H_