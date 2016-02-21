#ifndef _EASYTERRAIN3D_INTERFACE_DEMO_H_
#define _EASYTERRAIN3D_INTERFACE_DEMO_H_

#include <ee/Timer.h>

namespace eterrain3d
{

class IDemo
{
public:
	virtual ~IDemo() {}
	virtual void Load();
	virtual void Update();
	virtual void Draw() const;

protected:
	mutable int m_verts_per_frame;
	mutable int m_tris_per_frame;

private:
	ee::Timer m_timer;

}; // IDemo

}

#endif // _EASYTERRAIN3D_INTERFACE_DEMO_H_