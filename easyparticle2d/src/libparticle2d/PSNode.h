#ifndef _EASYPARTICLE2D_PS_NODE_H_
#define _EASYPARTICLE2D_PS_NODE_H_

namespace eparticle2d
{

class PSNode
{
public:
	void UpdateTime();
	float GetTime() const { return m_time; }

	static PSNode* Instance();

private:
	PSNode();

	void Init();

private:
	float m_time;

private:
	static PSNode* m_instance;

}; // PSNode

}

#endif // _EASYPARTICLE2D_PS_NODE_H_