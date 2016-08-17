#ifndef _EASYEDITOR_COMBINE_AOP_H_
#define _EASYEDITOR_COMBINE_AOP_H_

#include "AtomicOP.h"

namespace ee
{

class CombineAOP : public AtomicOP
{
public:

	void Insert(AtomicOP* atomic);

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<Sprite*>& sprs) const;

private:
	std::vector<AtomicOP*> m_atomics;

}; // CombineAOP

}

#endif // _EASYEDITOR_COMBINE_AOP_H_