#ifndef _TEST_DTEX_INTERFACE_TEST_H_
#define _TEST_DTEX_INTERFACE_TEST_H_

#include <easyejoy2d.h>

namespace tdtex
{

class ITest
{
public:
	virtual ~ITest() {}
	virtual void Load() = 0;
	virtual void Update() = 0;
	virtual void Draw() const = 0;

protected:
	static dtex_package* LoadPackage(const std::string& pkg, bool load_tex = false);

}; // ITest

}

#endif // _TEST_DTEX_INTERFACE_TEST_H_