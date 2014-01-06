#pragma once

#include <JSON/json.h>

namespace d2d
{
	class ISprite;
	class AbstractAtomicOP;

	class AtomicFactory
	{
	public:
		static AbstractAtomicOP* create(const Json::Value& val,
			const std::vector<ISprite*>& sprites);

	}; // AtomicFactory
}