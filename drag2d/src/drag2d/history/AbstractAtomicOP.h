#ifndef _DRAG2D_ABSTRACT_ATOMIC_OP_H_
#define _DRAG2D_ABSTRACT_ATOMIC_OP_H_

#include <json/json.h>

namespace d2d
{

class Sprite;

class AbstractAtomicOP
{
public:
	virtual ~AbstractAtomicOP() {}

	virtual void Undo() = 0;
	virtual void Redo() = 0;

	virtual Json::Value Store(const std::vector<Sprite*>& sprites) const = 0;

//	virtual void load(const Json::Value& val) = 0;

}; // AbstractAtomicOP

}

#endif // _DRAG2D_ABSTRACT_ATOMIC_OP_H_