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

		virtual void Undo();
		virtual void Redo();

		virtual Json::Value Store(const std::vector<d2d::Sprite*>& sprites) const {
			Json::Value ret;
			return ret;
		}

	private:
		Symbol* m_symbol;

		d2d::Vector m_src, m_dst;

	}; // ResizeAtomicOP
}