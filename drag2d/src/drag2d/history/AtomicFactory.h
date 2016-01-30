#pragma once

#include <json/json.h>

namespace d2d
{
	class Sprite;
	class AbstractAtomicOP;

	class AtomicFactory
	{
	public:
		static AbstractAtomicOP* create(const Json::Value& val,
			const std::vector<Sprite*>& sprites);

	}; // AtomicFactory
}