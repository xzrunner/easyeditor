#ifndef _EASYPARTICLE3D_PS_H_
#define _EASYPARTICLE3D_PS_H_

namespace eparticle3d
{

class PS
{
public:
	static PS* Instance();

private:
	PS();

	void Init();

private:
	static PS* m_instance;

}; // PS

}

#endif // _EASYPARTICLE3D_PS_H_