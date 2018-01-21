#pragma once

namespace eone
{

class Subject;
class VariantSet;

class Observer
{
public:
	virtual ~Observer() {}

	virtual void OnNotify(int sub_id, const VariantSet& variants) = 0;

}; // Observer

}