#pragma once

#include <drag2d.h>

namespace escale9
{
	class Symbol;

	class ResizeAtomicOP : public d2d::AbstractAtomicOP
	{
	public:
		ResizeAtomicOP(Symbol* symbol, const d2d::Vector& src, 
			const d2d::Vector& dst);

		virtual void undo();
		virtual void redo();

		virtual Json::Value store(const std::vector<d2d::ISprite*>& sprites) {
			Json::Value ret;
			return ret;
		}

	private:
		Symbol* m_symbol;

		d2d::Vector m_src, m_dst;

	}; // ResizeAtomicOP
}