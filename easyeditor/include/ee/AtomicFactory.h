#ifndef _EASYEDITOR_ATOMIC_FACTORY_H_
#define _EASYEDITOR_ATOMIC_FACTORY_H_

#include <json/json.h>

namespace ee
{

class Sprite;
class AtomicOP;

class AtomicFactory
{
public:
	static AtomicOP* Create(const Json::Value& val,
		const std::vector<Sprite*>& sprs);

}; // AtomicFactory

}

#endif // _EASYEDITOR_ATOMIC_FACTORY_H_