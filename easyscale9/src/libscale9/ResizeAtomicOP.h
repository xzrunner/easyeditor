#ifndef _EASYSCALE9_RESIZE_ATOMIC_OP_H_
#define _EASYSCALE9_RESIZE_ATOMIC_OP_H_

#include <ee/AtomicOP.h>

#include <SM_Vector.h>

namespace escale9
{

class Symbol;

class ResizeAtomicOP : public ee::AtomicOP
{
public:
	ResizeAtomicOP(const std::shared_ptr<Symbol>& sym, const sm::vec2& src, 
		const sm::vec2& dst);

	virtual void Undo() override;
	virtual void Redo() override;

	virtual Json::Value Store(const std::vector<ee::SprPtr>& sprs) const override {
		Json::Value ret;
		return ret;
	}

private:
	std::shared_ptr<Symbol> m_sym;

	sm::vec2 m_src, m_dst;

}; // ResizeAtomicOP

}

#endif // _EASYSCALE9_RESIZE_ATOMIC_OP_H_