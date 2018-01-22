#pragma once

#include "msg/MessageID.h"

namespace eone
{

class VariantSet;

class Observer
{
public:
	virtual ~Observer() {}

	virtual void OnNotify(MessageID msg, const VariantSet& variants) = 0;

}; // Observer

}