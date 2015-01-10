#ifndef _EASYTERRAIN3D_INTERFACE_DEMO_H_
#define _EASYTERRAIN3D_INTERFACE_DEMO_H_

namespace libterrain3d
{

class IDemo
{
public:
	virtual ~IDemo() {}
	virtual void Load() = 0;
	virtual void Draw() const = 0;

}; // IDemo

}

#endif // _EASYTERRAIN3D_INTERFACE_DEMO_H_