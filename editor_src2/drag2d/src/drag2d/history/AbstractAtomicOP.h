#pragma once

namespace d2d
{
	class AbstractAtomicOP
	{
	public:
		virtual ~AbstractAtomicOP() {}

		virtual void undo() = 0;
		virtual void redo() = 0;

	}; // AbstractAtomicOP
}

