#ifndef _EASYPARTICLE2D_PS_H_
#define _EASYPARTICLE2D_PS_H_

namespace eparticle2d
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

#endif // _EASYPARTICLE2D_PS_H_