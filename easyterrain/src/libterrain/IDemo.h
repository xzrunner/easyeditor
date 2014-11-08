#ifndef _EASYTERRAIN_INTERFACE_DEMO_H_
#define _EASYTERRAIN_INTERFACE_DEMO_H_

namespace libterrain
{

class IDemo
{
public:
	virtual ~IDemo() {}
	virtual void Load() = 0;
	virtual void Draw() const = 0;

}; // IDemo

}

#endif // _EASYTERRAIN_INTERFACE_DEMO_H_