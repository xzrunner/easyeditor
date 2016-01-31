#ifndef _EASYEDITOR_ATOMIC_OP_H_
#define _EASYEDITOR_ATOMIC_OP_H_

#include <json/json.h>

namespace ee
{

class Sprite;

class AtomicOP
{
public:
	virtual ~AtomicOP() {}

	virtual void Undo() = 0;
	virtual void Redo() = 0;

	virtual Json::Value Store(const std::vector<Sprite*>& sprites) const = 0;

//	virtual void load(const Json::Value& val) = 0;

}; // AtomicOP

}

#endif // _EASYEDITOR_ATOMIC_OP_H_