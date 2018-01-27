#pragma once

#include "ee0/MessageID.h"

namespace ee0
{

class VariantSet;

class Observer
{
public:
	virtual ~Observer() {}

	virtual void OnNotify(MessageID msg, const VariantSet& variants) = 0;

}; // Observer

}