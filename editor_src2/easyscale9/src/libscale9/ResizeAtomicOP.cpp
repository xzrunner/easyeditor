#include "ResizeAtomicOP.h"

namespace escale9
{
	ResizeAtomicOP::ResizeAtomicOP(d2d::Scale9Symbol* symbol,
		const d2d::Vector& src, const d2d::Vector& dst)
		: m_symbol(symbol)
		, m_src(src)
		, m_dst(dst)
	{
	}

	void ResizeAtomicOP::undo()
	{
		m_symbol->resize(m_src.x, m_src.y);
	}

	void ResizeAtomicOP::redo()
	{
		m_symbol->resize(m_dst.x, m_dst.y);
	}
}