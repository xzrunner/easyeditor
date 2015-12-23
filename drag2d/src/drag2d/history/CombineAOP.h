#ifndef _DRAG2D_COMBINE_AOP_H_
#define _DRAG2D_COMBINE_AOP_H_

#include "AbstractAtomicOP.h"

namespace d2d
{

class CombineAOP : public AbstractAtomicOP
{
public:

	void Insert(AbstractAtomicOP* atomic);

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<ISprite*>& sprites) const;

private:
	std::vector<AbstractAtomicOP*> m_atomics;

}; // CombineAOP

}

#endif // _DRAG2D_COMBINE_AOP_H_