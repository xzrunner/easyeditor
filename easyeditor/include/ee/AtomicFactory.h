#ifndef _EASYEDITOR_ATOMIC_FACTORY_H_
#define _EASYEDITOR_ATOMIC_FACTORY_H_

#include "Sprite.h"

#include <json/json.h>

namespace ee
{

class AtomicOP;

class AtomicFactory
{
public:
	static std::shared_ptr<AtomicOP> Create(const Json::Value& val,
		const std::vector<SprPtr>& sprs);

}; // AtomicFactory

}

#endif // _EASYEDITOR_ATOMIC_FACTORY_H_