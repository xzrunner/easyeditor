#pragma once

#include <json.h>

namespace d2d
{
	class ISprite;

	class AbstractAtomicOP
	{
	public:
		virtual ~AbstractAtomicOP() {}

		virtual void undo() = 0;
		virtual void redo() = 0;

		virtual Json::Value store(const std::vector<ISprite*>& sprites) = 0;

//		virtual void load(const Json::Value& val) = 0;

	}; // AbstractAtomicOP
}

