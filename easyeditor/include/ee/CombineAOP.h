#ifndef _EASYEDITOR_COMBINE_AOP_H_
#define _EASYEDITOR_COMBINE_AOP_H_

#include "AtomicOP.h"
#include "Sprite.h"

namespace ee
{

class CombineAOP : public AtomicOP
{
public:

	void Insert(AtomicOP* atomic);

	virtual void Undo() override;
	virtual void Redo() override;

	virtual Json::Value Store(const std::vector<SprPtr>& sprs) const override;

private:
	std::vector<AtomicOP*> m_atomics;

}; // CombineAOP

}

#endif // _EASYEDITOR_COMBINE_AOP_H_