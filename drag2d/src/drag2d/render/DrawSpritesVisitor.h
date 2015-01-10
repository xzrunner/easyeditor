#pragma once

#include "interfaces.h"

namespace d2d
{

class DrawSpritesVisitor : public IVisitor
{
public:
	virtual void visit(Object* object, bool& bFetchNext);

}; // DrawSpritesVisitor

}

